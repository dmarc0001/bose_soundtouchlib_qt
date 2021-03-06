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
    *lg << LDEBUG << "BoseCommServer::BoseCommServer: create timer..." << endl;
    dTimer = std::unique_ptr< DaemonTimer >( new DaemonTimer( config, this ) );
    *lg << LDEBUG << "BoseCommServer::BoseCommServer: connect timer events..." << endl;
    connect( dTimer.get(), &DaemonTimer::sigStartAlert, this, &BoseCommServer::onStartAlert );
    connect( dTimer.get(), &DaemonTimer::sigStopAlert, this, &BoseCommServer::onStopAlert );
    *lg << LDEBUG << "BoseCommServer::BoseCommServer: connect timer events...OK" << endl;
    *lg << LDEBUG << "BoseCommServer::BoseCommServer: create device discovering..." << endl;
    stDiscover = std::unique_ptr< SoundTouchDiscover >( new SoundTouchDiscover( config, this ) );
    *lg << LDEBUG << "BoseCommServer::BoseCommServer: create device discovering...OK" << endl;
  }

  /**
   * @brief BoseCommServer::~BoseCommServer
   */
  BoseCommServer::~BoseCommServer()
  {
    *lg << LDEBUG << "BoseCommServer::~BoseCommServer: destructor..." << endl;
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
      if ( currAlert->wait( 50000 ) )
      {
        *lg << LCRIT << "BoseCommServer::~BoseCommServer: Thread was not finished, timeout!" << endl;
        currAlert->exit();
      }
      currAlert->deleteLater();
      activeAlerts.removeOne( currAlert );
    }
    *lg << LINFO << "BoseCommServer::~BoseCommServer: destructor...OK" << endl;
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
        *lg << LCRIT << "BoseCommServer::reciveAsyncSignal: recived SIGINT/SIGTERM!" << endl;
        for ( auto &currAlert : activeAlerts )
        {
          currAlert->cancelAlert();
          if ( !currAlert->wait( 2500 ) )
          {
            *lg << LCRIT << "BoseCommServer::reciveAsyncSignal: Thread was not finished, timeout!" << endl;
          }
          QThread::msleep( 50 );
        }
        // das ende signalisieren
        *lg << LINFO << "BoseCommServer::reciveAsyncSignal: emit close signal!" << endl;
        emit closed();
        deleteLater();
        break;
#ifdef UNIX
      case SIGHUP:
        *lg << LINFO << "BoseCommServer::reciveAsyncSignal: recived SIGNAL to reload config!" << endl;
        break;
#endif
      default:
        *lg << LCRIT << QString( "BoseCommServer::reciveAsyncSignal: SIG Nr: %1" ).arg( signal ) << endl;
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
      *lg << LDEBUG << "BoseCommServer::configServer: commserver create server socket..." << endl;
      cServer = std::make_unique< QWebSocketServer >( QStringLiteral( "command server" ), QWebSocketServer::NonSecureMode, this );
      *lg << "BoseCommServer::configServer: commserver create server socket...OK" << endl;
      //
      // feststellen ob der Server gebunden wird
      //
      *lg << LDEBUG << "BoseCommServer::configServer: commserver try listen on socket..." << endl;
      if ( cServer->listen( QHostAddress( config->getBindaddr() ), static_cast< quint16 >( config->getBindport().toInt() ) ) )
      {
        *lg << LDEBUG << "BoseCommServer::configServer: commserver try listen on socket...OK" << endl;
        *lg << LINFO << "commserver is ready for connections..." << endl;
        //
        // signal für neue Verbindungen verbinden
        //
        *lg << LDEBUG << "BoseCommServer::configServer: commserver connect socket to slots..." << endl;
        connect( cServer.get(), &QWebSocketServer::newConnection, this, &BoseCommServer::onNewConnection );
        connect( cServer.get(), &QWebSocketServer::closed, this, &BoseCommServer::onClosedListening );
        *lg << LDEBUG << "BoseCommServer::configServer: commserver connect socket to slots...OK" << endl;
      }
      else
      {
        *lg << LCRIT << "commserver can't start!" << endl;
        // TODO: ALARM!
        return false;
      }
    }
    catch ( QException &ex )
    {
      // TODO: mach was!
      *lg << LCRIT << QString( "BoseCommServer::configServer" ).arg( ex.what() ) << endl;
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
        << endl;
    //
    // connect close
    //
    connect( handler.get(), &ConnectionHandler::closed, this, &BoseCommServer::onClientClosed );
    // in die Liste
    *lg << LDEBUG << "BoseCommServer::onNewConnection: commserver: new connection to list" << endl;
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
        << endl;
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
            << endl;
        break;
      }
    }
    *lg << LDEBUG << QString( "BoseCommServer::onClientClosed: commserver: open connections %1" ).arg( remoteConnections.count() )
        << endl;
  }

  /**
   * @brief BonNewConnectionlosedRemListening
   */
  void BoseCommServer::onClosedListening()
  {
    //
    // alles schliessen, fertig
    //
    *lg << LINFO << "BoseCommServer::onClosedListening: listening comserver socket closed." << endl;
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
      // lg->startLogging( LgThreshold::LG_DEBUG, config->getLogfile() );
      lg->startLogging( static_cast< LgThreshold >( config->getThreshold() ), config->getLogfileName(), config->getLogToConsole() );
      config->setLogger( lg );
      *lg << LINFO << "BoseCommServer::createLogger: logger created..." << endl;
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
    *lg << LDEBUG << QString( "BoseCommServer::onStartAlert: <%1>..." ).arg( alert.getName() ) << endl;
    if ( alert.getRunSince().isValid() )
    {
      *lg << LWARN << QString( "BoseCommServer::onStartAlert: <%1> alredy running. Abort starting..." ).arg( alert.getName() ) << endl;
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
    connect( currAlert, &BoseSoundAlert::finished, [this, currAlert]() { this->onAlertFinish( currAlert ); } );
    currAlert->start();
    activeAlerts.append( currAlert );
  }

  /**
   * @brief BoseCommServer::onStopAlert
   * @param alert
   */
  void BoseCommServer::onStopAlert( SingleAlertConfig &alert )
  {
    *lg << LDEBUG << QString( "BoseCommServer::onStopAlert: <%1>..." ).arg( alert.getName() ) << endl;
    for ( auto currAlert : activeAlerts )
    {
      if ( alert.getName().compare( currAlert->getAlertName() ) )
      {
        //
        // diesen alarm beenden
        //
        currAlert->cancelAlert();
        break;
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
    *lg << LDEBUG << QString( "BoseCommServer::onAlertFinish: <%1>..." ).arg( alertName ) << endl;
    //
    // der normale weg den Alarm zu entsorgen.
    // bei Cancel via SIGNAL ist der dann schon aus der Liste
    // und es passiert hier nix
    //
    for ( auto currAlert : activeAlerts )
    {
      if ( alert == currAlert )
      {
        //
        // diesen alarm loeschen
        //
        *lg << LDEBUG << QString( "BoseCommServer::onAlertFinish: <%1> delete..." ).arg( alertName ) << endl;
        activeAlerts.removeOne( alert );
        alert->deleteLater();
        break;
      }
    }
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
  }
}  // namespace bose_commserver
