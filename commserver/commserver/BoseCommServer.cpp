#include "BoseCommServer.hpp"
#include <QtDebug>

namespace bose_commserver
{
  /**
   * @brief BoseCommServer::BoseCommServer
   * @param dconfig
   * @param parent
   */
  BoseCommServer::BoseCommServer( std::shared_ptr< DaemonConfig > dconfig, QObject *parent ) : QObject( parent ), config( dconfig )
  {
    if ( config->getIsDebug() )
    {
      qDebug() << "commserver object created...";
      qDebug() << "create serversocket...";
    }
    createServer();
    if ( config->getIsDebug() )
      qDebug() << "create serversocket...OK";
  }

  /**
   * @brief BoseCommServer::~BoseCommServer
   */
  BoseCommServer::~BoseCommServer()
  {
    cServer->close();
    qDeleteAll( remoteSockets.begin(), remoteSockets.end() );
  }

  /**
   * @brief BoseCommServer::createServer
   * @return
   */
  bool BoseCommServer::createServer()
  {
    try
    {
      qDebug() << "commserver server socket create...";
      cServer = std::unique_ptr< QWebSocketServer >(
          new QWebSocketServer( QStringLiteral( "command server" ), QWebSocketServer::NonSecureMode, this ) );
      qDebug() << "commserver server socket create...OK";
      //
      // feststellen ob der Server gebunden wird
      //
      // if ( cServer->listen( QHostAddress::Any, static_cast< quint16 >( config->getBindport().toInt() ) ) )
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
    QWebSocket *nSock = cServer->nextPendingConnection();
    qDebug() << "commserver: new Connection: " << nSock->peerName() << nSock->origin();
    connect( nSock, &QWebSocket::textMessageReceived, this, &BoseCommServer::remProcTextMessage );
    connect( nSock, &QWebSocket::binaryMessageReceived, this, &BoseCommServer::remProcBinaryMessage );
    connect( nSock, &QWebSocket::disconnected, this, &BoseCommServer::remSocketDisconnected );
    // in die Liste
    remoteSockets << nSock;
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

  /**
   * @brief BoseCommServer::remProcTextMessage
   * @param msg
   */
  void BoseCommServer::remProcTextMessage( QString msg )
  {
    //
    // wer war das?
    //
    QWebSocket *nSock = qobject_cast< QWebSocket * >( sender() );
    qDebug() << "commserver: msg recived: " << msg << " from: " << nSock->peerAddress().toString() << ", " << nSock->peerPort();
    nSock->sendTextMessage( msg );
  }

  /**
   * @brief BoseCommServer::remProcBinaryMessage
   * @param msg
   */
  void BoseCommServer::remProcBinaryMessage( QByteArray msg )
  {
    //
    // wer war das?
    //
    QWebSocket *nSock = qobject_cast< QWebSocket * >( sender() );
    qDebug() << "commserver: binary msg recived from: " << nSock->peerAddress().toString() << ", " << nSock->peerPort();
  }

  /**
   * @brief BoseCommServer::remSocketDisconnected
   */
  void BoseCommServer::remSocketDisconnected()
  {
    //
    // wer war das?
    //
    QWebSocket *nSock = qobject_cast< QWebSocket * >( sender() );
    qDebug() << "commserver: closed remote connection from: " << nSock->peerAddress().toString() << ", " << nSock->peerPort();
    // TODO: noch was intelligenten machen
    remoteSockets.removeAll( nSock );
    nSock->deleteLater();
  }

}  // namespace bose_commserver
