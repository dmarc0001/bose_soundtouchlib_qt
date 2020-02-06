#include "BoseCommServer.hpp"
#include <QtDebug>

namespace bose_commserver
{
  /**
   * @brief BoseCommServer::BoseCommServer
   * @param dconfig
   * @param parent
   */
  BoseCommServer::BoseCommServer( std::shared_ptr< DaemonConfig > dconfig, QObject *parent )
      : QObject( parent ), config( dconfig )  // , cServer( QStringLiteral( "command server" ), QWebSocketServer::NonSecureMode, this )
  {
    if ( config->getIsDebug() )
    {
      qDebug() << "commserver object created...";
      qDebug() << "create serversocket...";
    }
    if ( !createLogger() )
    {
      //
      // was drastisches machen, exception werfen oder so
      //
      emit closed();
    }
    else
    {
      configServer();
      if ( config->getIsDebug() )
        qDebug() << "create serversocket...OK";
    }
  }

  /**
   * @brief BoseCommServer::~BoseCommServer
   */
  BoseCommServer::~BoseCommServer()
  {
    cServer->close();
    // for ( std::shared_ptr< ConnectionHandler > handler : remoteConnections )
    // {
    // alle durchgehen, die Liste selber wird vom Destruktor gelöscht
    // die Handlerzeiger machen das für sich selbstständig
    //   handler->disconnectWebsocket();
    // }
  }

  /**
   * @brief BoseCommServer::createServer
   * @return
   */
  bool BoseCommServer::configServer()
  {
    try
    {
      qDebug() << "commserver server socket create...";
      cServer = std::make_unique< QWebSocketServer >( QStringLiteral( "command server" ), QWebSocketServer::NonSecureMode, this );
      qDebug() << "commserver server socket create...OK";
      //
      // feststellen ob der Server gebunden wird
      //
      if ( cServer->listen( QHostAddress( config->getBindaddr() ), static_cast< quint16 >( config->getBindport().toInt() ) ) )
      {
        qDebug() << "Server socket started!";
        //
        // signal für neue Verbindungen verbinden
        //
        qDebug() << "commserver server socket connect slots...";
        connect( cServer.get(), &QWebSocketServer::newConnection, this, &BoseCommServer::newRemConnection );
        connect( cServer.get(), &QWebSocketServer::closed, this, &BoseCommServer::closedRemListening );
        qDebug() << "commserver server socket connect slots...OK";
      }
      else
      {
        qDebug() << "Server socket could not start";
        // TODO: ALARM!
        return false;
      }
    }
    catch ( QException ex )
    {
      // TODO: mach weas!
      return false;
    }
    return true;
  }

  /**
   * @brief BoseCommServer::newRemConnection
   */
  void BoseCommServer::newRemConnection()
  {
    //
    // gib mal den Zeiger auf die neue Verbindung her
    //
    std::shared_ptr< QWebSocket > nSock = std::shared_ptr< QWebSocket >( cServer->nextPendingConnection() );
    // QWebSocket *nSock = cServer->nextPendingConnection();
    std::shared_ptr< ConnectionHandler > handler = std::make_shared< ConnectionHandler >( ConnectionHandler( config, nSock, this ) );
    qDebug() << "commserver: new Connection: " << nSock->peerName() << nSock->origin();
    connect( nSock.get(), &QWebSocket::textMessageReceived, handler.get(), &ConnectionHandler::remProcTextMessage );
    connect( nSock.get(), &QWebSocket::binaryMessageReceived, handler.get(), &ConnectionHandler::remProcBinaryMessage );
    connect( nSock.get(), &QWebSocket::disconnected, handler.get(), &ConnectionHandler::remSocketDisconnected );
    // in die Liste
    remoteConnections << handler;
  }

  /**
   * @brief BoseCommServer::closedRemListening
   */
  void BoseCommServer::closedRemListening()
  {
    //
    // alles schliessen, fertig
    //
    qDebug() << "commserver: listening socket closed...";
    // TODO: bestehende sitzungen beednen/schiessen
    // TODO: emit commandClosedSignal
  }

  bool BoseCommServer::createLogger()
  {
    //
    // erzeuge einen Logger, untersuche zunächst ob es das Verzeichnis gibt
    //
    QString logDirStr = config->getLogpath();
    QDir logDir( logDirStr );
    // Logger erzeugen
    std::shared_ptr< Logger > lg = std::make_shared< Logger >( Logger() );
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
          qDebug() << "BoseCommServer::createLogger -> path NOT created!";
          return ( false );
        }
      }
      //
      // das wird wohl klappen
      //
      lg->startLogging( static_cast< LgThreshold >( config->getThreshold() ), config->getLogfile() );
      config->setLogger( lg );
      return ( true );
    }
    //
    // Fehler, melde das dem User
    //
    qDebug() << "BoseCommServer::createLogger -> NOT created!";
    return ( false );
  }

}  // namespace bose_commserver
