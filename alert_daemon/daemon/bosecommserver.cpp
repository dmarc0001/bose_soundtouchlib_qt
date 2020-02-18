//
// Websocket Kommunikation
//
// der Serverprozess vermittelt
// - zwischen dem Web Frontend (dem User) und dem Interface für die Konfiguration
// - kommunizieert zwischen dem Timermodul und den Geräten
//
#include "bosecommserver.hpp"

#include <sys/socket.h>
#include <unistd.h>

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
    qDebug() << "create serversocket...";
    qDebug() << "config hash <" << dconfig->getConfigHash() << ">";
    if ( !createLogger() )
    {
      //
      // was drastisches machen, exception werfen oder so
      //
      qCritical() << "can't create logger, abort commserver....";
      emit closed();
    }
    else
    {
      configServer();
      qDebug() << "LOGFILE" << config->getLogfileName();
      lg->debug( "BoseCommServer::BoseCommServer: serversocket created..." );
      qDebug() << "create serversocket...OK";
    }
    // if ( ::socketpair( AF_UNIX, SOCK_STREAM, 0, sighupFd ) )
    //  qFatal( "Couldn't create HUP socketpair" );
    if ( ::socketpair( AF_UNIX, SOCK_STREAM, 0, sigtermFd ) )
    {
      lg->crit( "Couldn't create TERM socketpair, cant castch SIG_TERM." );
    }
    lg->debug( "BoseCommServer::BoseCommServer: signalhandling init..." );
    // snHup = new QSocketNotifier( sighupFd[ 1 ], QSocketNotifier::Read, this );
    // connect( snHup, SIGNAL( activated( int ) ), this, SLOT( handleSigHup() ) );
    snTerm = std::unique_ptr< QSocketNotifier >( new QSocketNotifier( sigtermFd[ 1 ], QSocketNotifier::Read, this ) );
    connect( snTerm.get(), &QSocketNotifier::activated, this, &BoseCommServer::onHandleSigTerm );
    lg->debug( "BoseCommServer::BoseCommServer: signalhandling init...OK" );
    //
    lg->debug( "BoseCommServer::BoseCommServer: create timer..." );
    dTimer = std::unique_ptr< DaemonTimer >( new DaemonTimer( config, this ) );
    lg->debug( "BoseCommServer::BoseCommServer: connect timer events..." );
    connect( dTimer.get(), &DaemonTimer::sigStartAlert, this, &BoseCommServer::onStartAlert );
    connect( dTimer.get(), &DaemonTimer::sigStopAlert, this, &BoseCommServer::onStopAlert );
    lg->debug( "BoseCommServer::BoseCommServer: connect timer events...OK" );
  }

  /**
   * @brief BoseCommServer::~BoseCommServer
   */
  BoseCommServer::~BoseCommServer()
  {
    lg->info( "BoseCommServer::~BoseCommServer: destructor..." );
    cServer->close();
    for ( std::shared_ptr< ConnectionHandler > handler : remoteConnections )
    {
      //
      // alle durchgehen, die Liste selber wird vom Destruktor gelöscht
      // die Einträge aus der Liste löschen sich selber
      //
      handler->disconnectWebsocket();
    }
    lg->shutdown();
  }

  /*
  void BoseCommServer::hupSignalHandler( int )
  {
    char a = 1;
    ::write( sighupFd[ 0 ], &a, sizeof( a ) );
  }
  */
  void BoseCommServer::termSignalHandler( int )
  {
    char a = 1;
    ::write( sigtermFd[ 0 ], &a, sizeof( a ) );
  }

  void BoseCommServer::onHandleSigTerm()
  {
    snTerm->setEnabled( false );
    char tmp;
    ::read( sigtermFd[ 1 ], &tmp, sizeof( tmp ) );

    // do Qt stuff

    snTerm->setEnabled( true );
  }

  /**
   * @brief BoseCommServer::createServer
   * @return
   */
  bool BoseCommServer::configServer()
  {
    try
    {
      lg->debug( "BoseCommServer::configServer: commserver create server socket..." );
      cServer = std::make_unique< QWebSocketServer >( QStringLiteral( "command server" ), QWebSocketServer::NonSecureMode, this );
      lg->debug( "BoseCommServer::configServer: commserver create server socket...OK" );
      //
      // feststellen ob der Server gebunden wird
      //
      lg->debug( "BoseCommServer::configServer: commserver try listen on socket..." );
      if ( cServer->listen( QHostAddress( config->getBindaddr() ), static_cast< quint16 >( config->getBindport().toInt() ) ) )
      {
        lg->debug( "BoseCommServer::configServer: commserver try listen on socket...OK" );
        lg->info( "commserver is ready for connections..." );
        //
        // signal für neue Verbindungen verbinden
        //
        lg->debug( "BoseCommServer::configServer: commserver connect socket to slots..." );
        connect( cServer.get(), &QWebSocketServer::newConnection, this, &BoseCommServer::onNewConnection );
        connect( cServer.get(), &QWebSocketServer::closed, this, &BoseCommServer::onClosedListening );
        lg->debug( "BoseCommServer::configServer: commserver connect socket to slots...OK" );
      }
      else
      {
        lg->crit( "commserver can't start!" );
        // TODO: ALARM!
        return false;
      }
    }
    catch ( QException &ex )
    {
      // TODO: mach was!
      lg->crit( QString( "BoseCommServer::configServer" ).arg( ex.what() ) );
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
    std::shared_ptr< ConnectionHandler > handler =
        std::shared_ptr< ConnectionHandler >( new ConnectionHandler( config, nSock, this ) );
    lg->info( QString( "BoseCommServer::onNewConnection: commserver: new connection from: %1, id: %2" )
                  .arg( nSock->peerAddress().toString() )
                  .arg( handler->getCurrentHandlerNum(), 12, 10, QLatin1Char( '0' ) ) );
    //
    // connect close
    //
    connect( handler.get(), &ConnectionHandler::closed, this, &BoseCommServer::onClientClosed );
    // in die Liste
    lg->debug( "BoseCommServer::onNewConnection: commserver: new connection to list" );
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
    lg->info( QString( "BoseCommServer::onClientClosed: commserver: closed connection from: %1, id: %2" )
                  .arg( handler->getNSock()->peerAddress().toString() )
                  .arg( toDeleteHandlerId, 12, 10, QLatin1Char( '0' ) ) );
    for ( std::shared_ptr< ConnectionHandler > currHandler : remoteConnections )
    {
      if ( toDeleteHandlerId == currHandler->getCurrentHandlerNum() )
      {
        //
        // hurraaaaa, hau weg das Ding
        // entfernen aus der Liste
        //
        bool wasRemoved = remoteConnections.removeOne( currHandler );
        lg->debug( QString( "BoseCommServer::onClientClosed: commserver: closed connection from: %1, id: %2 deleted %3" )
                       .arg( handler->getNSock()->peerAddress().toString() )
                       .arg( toDeleteHandlerId, 12, 10, QLatin1Char( '0' ) )
                       .arg( wasRemoved ) );
        break;
      }
    }
    lg->debug( QString( "BoseCommServer::onClientClosed: commserver: open connections %1" ).arg( remoteConnections.count() ) );
  }

  /**
   * @brief BonNewConnectionlosedRemListening
   */
  void BoseCommServer::onClosedListening()
  {
    //
    // alles schliessen, fertig
    //
    lg->info( "BoseCommServer::onClosedListening: listening comserver socket closed." );
    // remoteConnections
    // TODO: bestehende sitzungen beednen/schiessen
    // TODO: emit commandClosedSignal
  }

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
    lg = std::shared_ptr< Logger >( new Logger() );
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
      lg->startLogging( static_cast< LgThreshold >( config->getThreshold() ), config->getLogfileName() );
      config->setLogger( lg );
      lg->debug( "BoseCommServer::createLogger: logger created..." );
      return ( true );
    }
    //
    // Fehler, melde das dem User
    //
    return ( false );
  }

  void BoseCommServer::onStartAlert( SingleAlertConfig &alert )
  {
    lg->debug( QString( "BoseCommServer::onStartAlert: <%1>..." ).arg( alert.getName() ) );
  }

  void BoseCommServer::onStopAlert( SingleAlertConfig &alert )
  {
    lg->debug( QString( "BoseCommServer::onStopAlert: <%1>..." ).arg( alert.getName() ) );
  }

}  // namespace bose_commserver
