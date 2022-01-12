//
// Websocket Kommunikation
//
// der Serverprozess vermittelt
// - zwischen dem Web Frontend (dem User) und dem Interface für die Konfiguration
// - kommunizieert zwischen dem Timermodul und den Geräten
//
#include "bosecommserver.hpp"

#include <QtDebug>

namespace bose_commserver
{
  /**
   * @brief BoseCommServer::BoseCommServer
   * @param dconfig
   * @param parent
   */
  BoseCommServer::BoseCommServer( std::shared_ptr< AlertAppConfig > dconfig, QObject *parent ) : QObject( parent ), config( dconfig )
  {
    qDebug() << "commserver object created...";
    qDebug() << "config hash <" << dconfig->getConfigHash() << ">";
    if ( !createLogger() )
    {
      //
      // was drastisches machen, exception werfen oder so
      //
      qCritical() << "can't create logger, abort commserver....";
      throw NoLoggerException( "cant't create logger!" );
    }
    else
    {
      configServer();
      qDebug() << "BoseCommServer::BoseCommServer: LOGFILE" << config->getLogfileName();
      *lg << LDEBUG << "BoseCommServer::BoseCommServer: serversocket created...";
    }
    *lg << LDEBUG << "BoseCommServer::BoseCommServer: create timer..." << Qt::endl;
    dTimer = std::unique_ptr< DaemonTimer >( new DaemonTimer( config, this ) );
    *lg << LDEBUG << "BoseCommServer::BoseCommServer: connect timer events..." << Qt::endl;
    connect( dTimer.get(), &DaemonTimer::sigStartAlert, this, &BoseCommServer::onStartAlert );
    connect( dTimer.get(), &DaemonTimer::sigStopAlert, this, &BoseCommServer::onStopAlert );
    *lg << LDEBUG << "BoseCommServer::BoseCommServer: connect timer events...OK" << Qt::endl;
    *lg << LDEBUG << "BoseCommServer::BoseCommServer: create device discovering..." << Qt::endl;
    stDiscover = std::unique_ptr< SoundTouchDiscover >( new SoundTouchDiscover( config, this ) );
    *lg << LDEBUG << "BoseCommServer::BoseCommServer: create device discovering...OK" << Qt::endl;
  }

  /**
   * @brief BoseCommServer::~BoseCommServer
   */
  BoseCommServer::~BoseCommServer()
  {
    *lg << LDEBUG << "BoseCommServer::~BoseCommServer: destructor..." << Qt::endl;
    stDiscover->cancel();
    dTimer->stopTimer();
    cServer->close();
    for ( auto handler : remoteConnections )
    {
      //
      // alle durchgehen, die Liste selber wird vom Destruktor gelöscht
      // die Einträge aus der Liste löschen sich selber
      //
      handler->disconnectWebsocket();
    }
    //
    // Alarme abwürgen
    //
    for ( auto &currAlert : activeAlerts )
    {
      currAlert->cancelAlert();
      // etwas zeit geben
      for ( quint8 i = 0; i < 30; i++ )
      {
        QThread::msleep( 100 );
        // ist das erledigt?
        if ( currAlert->isFinished() )
          break;
      }
      //
      // wenn der Alarm noch nicht erledigt ist, warnen
      //
      if ( !currAlert->isFinished() )
      {
        *lg << LCRIT << "BoseCommServer::~BoseCommServer: alert was not finished, timeout!" << Qt::endl;
      }
      currAlert->deleteLater();
      activeAlerts.removeOne( currAlert );
    }
    *lg << LINFO << "BoseCommServer::~BoseCommServer: destructor...OK" << Qt::endl;
    lg->shutdown();
  }

  /**
   * @brief BoseCommServer::reciveAsyncSignal
   * @param signal
   */
  void BoseCommServer::reciveSignal( int signal )
  {
    switch ( signal )
    {
      case SIGINT:
      case SIGTERM:
        dTimer->stopTimer();
        // config->getConfigHash();
        *lg << LCRIT << "BoseCommServer::reciveAsyncSignal: recived SIGINT/SIGTERM!" << Qt::endl;
        //
        // alle Alarme beenden
        //
        for ( auto &currAlert : activeAlerts )
        {
          *lg << LINFO << "Cancel running alert <" << currAlert->getAlertName() << ">..." << Qt::endl;
          currAlert->cancelAlert();
          // etwas zeit geben
          for ( quint8 i = 0; i < 30; i++ )
          {
            QThread::msleep( 100 );
            // ist das erledigt?
            if ( currAlert->isFinished() )
              break;
          }
          if ( !currAlert->isFinished() )
          {
            *lg << LCRIT << "BoseCommServer::reciveAsyncSignal: alert was not finished, timeout!" << Qt::endl;
          }
          *lg << LINFO << "Canceled running alert <" << currAlert->getAlertName() << ">...OK" << Qt::endl;
          QThread::msleep( 50 );
        }
        //
        // das ende signalisieren
        //
        *lg << LINFO << "BoseCommServer::reciveAsyncSignal: emit close signal!" << Qt::endl;
        emit closed();
        deleteLater();
        break;
#ifdef UNIX
      case SIGHUP:
        *lg << LINFO << "BoseCommServer::reciveAsyncSignal: recived SIGNAL to reload config!" << Qt::endl;
        break;
#endif
      default:
        *lg << LCRIT << QString( "BoseCommServer::reciveAsyncSignal: SIG Nr: %1" ).arg( signal ) << Qt::endl;
    }
  }

  /**
   * @brief BoseCommServer::createServer
   * @return
   */
  bool BoseCommServer::configServer()
  {
    try
    {
      *lg << LDEBUG << "BoseCommServer::configServer: commserver create server socket..." << Qt::endl;
      cServer = std::make_unique< QWebSocketServer >( QStringLiteral( "command server" ), QWebSocketServer::NonSecureMode, this );
      *lg << "BoseCommServer::configServer: commserver create server socket...OK" << Qt::endl;
      //
      // feststellen ob der Server gebunden wird
      //
      *lg << LDEBUG << "BoseCommServer::configServer: commserver try listen on socket..." << Qt::endl;
      if ( cServer->listen( QHostAddress( config->getBindaddr() ), static_cast< quint16 >( config->getBindport().toInt() ) ) )
      {
        *lg << LDEBUG << "BoseCommServer::configServer: commserver try listen on socket...OK" << Qt::endl;
        *lg << LINFO << "commserver is ready for connections..." << Qt::endl;
        //
        // signal für neue Verbindungen verbinden
        //
        *lg << LDEBUG << "BoseCommServer::configServer: commserver connect socket to slots..." << Qt::endl;
        connect( cServer.get(), &QWebSocketServer::newConnection, this, &BoseCommServer::onNewConnection );
        connect( cServer.get(), &QWebSocketServer::closed, this, &BoseCommServer::onClosedListening );
        *lg << LDEBUG << "BoseCommServer::configServer: commserver connect socket to slots...OK" << Qt::endl;
      }
      else
      {
        *lg << LCRIT << "commserver can't start!" << Qt::endl;
        // TODO: ALARM!
        return false;
      }
    }
    catch ( QException &ex )
    {
      // TODO: mach was!
      *lg << LCRIT << QString( "BoseCommServer::configServer" ).arg( ex.what() ) << Qt::endl;
      return false;
    }
    return true;
  }

  /**
   * @brief BoseCommServer::newRemConnection
   */
  void BoseCommServer::onNewConnection()
  {
    QMutexLocker locker( &qMutex );
    //
    // gib mal den Zeiger auf die neue Verbindung her
    //
    std::shared_ptr< QWebSocket > nSock = std::shared_ptr< QWebSocket >( cServer->nextPendingConnection() );
    //
    // std::make_shared< ConnectionHandler > geht hier nicht, da er ein Objekt erzeugt und dann
    // kopiert. Das geht schief bei einem übergebenen Socket :-(
    //
    auto handler = std::shared_ptr< ConnectionHandler >( new ConnectionHandler( config, nSock, this ) );
    *lg << LINFO
        << QString( "BoseCommServer::onNewConnection: commserver: new connection from: %1, id: %2" )
               .arg( nSock->peerAddress().toString() )
               .arg( handler->getCurrentHandlerNum(), 12, 10, QLatin1Char( '0' ) )
        << Qt::endl;
    //
    // connect close
    //
    connect( handler.get(), &ConnectionHandler::closed, this, &BoseCommServer::onClientClosed );
    // in die Liste
    *lg << LDEBUG << "BoseCommServer::onNewConnection: commserver: new connection to list" << Qt::endl;
    remoteConnections << handler;
  }

  /**
   * @brief BoseCommServer::onClientClosed Der Handler hat die Verbindung gewschlossen/verloren
   * @param handler
   */
  void BoseCommServer::onClientClosed( const ConnectionHandler *handler )
  {
    QMutexLocker locker( &qMutex );
    qlonglong toDeleteHandlerId = handler->getCurrentHandlerNum();
    *lg << LINFO
        << QString( "BoseCommServer::onClientClosed: commserver: closed connection from: %1, id: %2" )
               .arg( handler->getNSock()->peerAddress().toString() )
               .arg( toDeleteHandlerId, 12, 10, QLatin1Char( '0' ) )
        << Qt::endl;
    for ( auto currHandler : remoteConnections )
    {
      if ( toDeleteHandlerId == currHandler->getCurrentHandlerNum() )
      {
        //
        // hurraaaaa, hau weg das Ding
        // entfernen aus der Liste
        //
        bool wasRemoved = remoteConnections.removeOne( currHandler );
        *lg << LDEBUG
            << QString( "BoseCommServer::onClientClosed: commserver: closed connection from: %1, id: %2 deleted %3" )
                   .arg( handler->getNSock()->peerAddress().toString() )
                   .arg( toDeleteHandlerId, 12, 10, QLatin1Char( '0' ) )
                   .arg( wasRemoved )
            << Qt::endl;
        break;
      }
    }
    *lg << LDEBUG << QString( "BoseCommServer::onClientClosed: commserver: open connections %1" ).arg( remoteConnections.count() )
        << Qt::endl;
  }

  /**
   * @brief BonNewConnectionlosedRemListening
   */
  void BoseCommServer::onClosedListening()
  {
    //
    // alles schliessen, fertig
    //
    *lg << LINFO << "BoseCommServer::onClosedListening: listening comserver socket closed." << Qt::endl;
    // remoteConnections
    // TODO: bestehende sitzungen beednen/schiessen
    // TODO: emit commandClosedSignal
  }

  /**
   * @brief BoseCommServer::createLogger
   * @return
   */
  bool BoseCommServer::createLogger()
  {
    //
    // erzeuge einen Logger, untersuche zunächst ob es das Verzeichnis gibt
    //
    QString logDirStr = config->getLogfilePath();
    QDir logDir( logDirStr );
    // Logger erzeugen
    // wg Copycontruktor eher nicht...
    // lg = std::make_shared< Logger >( Logger() );
    //
    lg = LoggerPtr( new Logger() );
    if ( lg )
    {
      //
      // gibt es das Verzeichnis
      //
      if ( !logDir.exists() )
      {
        if ( !QDir().mkpath( logDirStr ) )
        {
          //
          // Das ging schief
          //
          qCritical() << "BoseCommServer::createLogger -> path NOT created!";
          return ( false );
        }
      }
      //
      // das wird wohl klappen
      //
      QString fullLogFileName = logDirStr.append( QDir::separator() ).append( config->getLogfileName() );
      lg->startLogging( static_cast< LgThreshold >( config->getThreshold() ), fullLogFileName, config->getLogToConsole() );
      config->setLogger( lg );
      *lg << LINFO << "BoseCommServer::createLogger: logger created..." << Qt::endl;
      return ( true );
    }
    //
    // Fehler, melde das dem User
    //
    return ( false );
  }

  /**
   * @brief BoseCommServer::onStartAlert
   * @param alert
   */
  void BoseCommServer::onStartAlert( SingleAlertConfig &alert )
  {
    *lg << LDEBUG << QString( "BoseCommServer::onStartAlert: <%1>..." ).arg( alert.getName() ) << Qt::endl;
    if ( alert.getRunSince().isValid() )
    {
      *lg << LWARN << QString( "BoseCommServer::onStartAlert: <%1> alredy running. Abort starting..." ).arg( alert.getName() )
          << Qt::endl;
      return;
    }
    //
    // markieren!
    //
    alert.setRunSince( QDateTime::currentDateTime() );
    //
    // einen Alarm einfügen, der Konstruktor startet diesen gleich
    //
    BoseSoundAlert *currAlert = new BoseSoundAlert( config, alert, lg, this );
    //
    // verbinde das finish Signal des alarms mit der slot funktion via lambda Funktion
    //
    connect( currAlert, &BoseSoundAlert::onFinish, [this, currAlert]() { this->onAlertFinish( currAlert ); } );
    activeAlerts.append( currAlert );
    *lg << LDEBUG << "BoseCommServer::onStartAlert: active alerts now <" << BoseSoundAlert::getAlertCount() << ">" << Qt::endl;
  }

  /**
   * @brief BoseCommServer::onStopAlert
   * @param alert
   */
  void BoseCommServer::onStopAlert( SingleAlertConfig &alert )
  {
    *lg << LDEBUG << QString( "BoseCommServer::onStopAlert: <%1>..." ).arg( alert.getName() ) << Qt::endl;
    for ( auto currAlert : activeAlerts )
    {
      if ( alert.getName().compare( currAlert->getAlertName() ) )
      {
        //
        // diesen alarm beenden
        //
        currAlert->cancelAlert();
        // ist QLIST, iterator bleibt gültig
        activeAlerts.removeOne( currAlert );
        return;
      }
    }
  }

  /**
   * @brief BoseCommServer::onAlertFinish
   * @param alName
   */
  void BoseCommServer::onAlertFinish( BoseSoundAlert *alert )
  {
    QString alertName = alert->getAlertName();
    *lg << LDEBUG << QString( "BoseCommServer::onAlertFinish: <%1>..." ).arg( alertName ) << Qt::endl;
    //
    // die Markierung in der Config wieder entfernen
    //
    AlertListPtr alerts = config->getAlConfigs();
    for ( auto currAlert : *alerts )
    {
      if ( alertName.compare( currAlert.getName() ) )
      {
        //
        // demarkieren
        //
        currAlert.setRunSince( QDateTime() );
        break;
      }
    }
    *lg << LDEBUG << "BoseCommServer::onAlertFinish: active alerts now <" << BoseSoundAlert::getAlertCount() << ">" << Qt::endl;
  }
}  // namespace bose_commserver
