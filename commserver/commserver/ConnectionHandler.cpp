#include "ConnectionHandler.hpp"

#include <QDebug>

namespace bose_commserver
{
  ulong ConnectionHandler::handlerNumberCounter{0};

  /**
   * @brief ConnectionHandler::ConnectionHandler
   * @param dconfig
   * @param theSock
   * @param parent
   */
  ConnectionHandler::ConnectionHandler( std::shared_ptr< DaemonConfig > dconfig,
                                        std::shared_ptr< QWebSocket > theSock,
                                        QObject *parent )
      : QObject( parent )
      , currentHandler( ConnectionHandler::handlerNumberCounter++ )
      , config( dconfig )
      , connected( false )
      , nSock( theSock )
  {
    //
    qDebug() << "ConnectionHandler: created handler id: " << currentHandler;
    init();
  }

  /**
   * @brief ConnectionHandler::ConnectionHandler
   * @param cp
   */
  ConnectionHandler::ConnectionHandler( const ConnectionHandler &cp ) : currentHandler( cp.currentHandler )
  {
    qDebug() << "ConnectionHandler: copyconstructor created handler id: " << currentHandler;
    config = cp.config;
    connected = cp.connected;
    nSock = cp.nSock;
    init();
  }

  void ConnectionHandler::init()
  {
    qDebug() << "ConnectionHandler: connect slots...";
    connect( nSock.get(), &QWebSocket::textMessageReceived, this, &ConnectionHandler::onRecTextMessage );
    connect( nSock.get(), &QWebSocket::binaryMessageReceived, this, &ConnectionHandler::onRecBinaryMessage );
    connect( nSock.get(), &QWebSocket::disconnected, this, &ConnectionHandler::onDisconnected );
  }

  /**
   * @brief ConnectionHandler::~ConnectionHandler
   */
  ConnectionHandler::~ConnectionHandler()
  {
    qDebug() << "ConnectionHandler: destructor handler id: " << currentHandler;
    /*
    // Websocket verbindung beenden
    if ( nSock->isValid() )
      if ( connected )
      {
        nSock->close();
        connected = false;
      }
    nSock->deleteLater();
    */
  }

  /**
   * @brief ConnectionHandler::getCurrentHandler
   * @return
   */
  ulong ConnectionHandler::getCurrentHandler() const
  {
    return currentHandler;
  }

  /**
   * @brief ConnectionHandler::disconnectWebsocket
   */
  void ConnectionHandler::disconnectWebsocket()
  {
    qDebug() << "ConnectionHandler: disconnect handler id: " << currentHandler;
    // Websocket verbindung beenden
    if ( nSock->isValid() )
      if ( connected )
      {
        nSock->close();
      }
    connected = false;
    nSock = nullptr;
  }

  /**
   * @brief ConnectionHandler::onRecTextMessage
   * @param msg
   */
  void ConnectionHandler::onRecTextMessage( QString msg )
  {
    qDebug() << "ConnectionHandler: recived text handler id: " << currentHandler;
    //
    // wer war das?
    //
    // QWebSocket *nSock = qobject_cast< QWebSocket * >( sender() );
    qDebug() << "commserver: msg recived: " << msg << " from: " << nSock->peerAddress().toString() << ", " << nSock->peerPort();
    nSock->sendTextMessage( "TEXT ACKNOWLEDGED" );
  }

  /**
   * @brief ConnectionHandler::onRecBinaryMessage
   */
  void ConnectionHandler::onRecBinaryMessage( QByteArray )
  {
    qDebug() << "ConnectionHandler: recived binary handler id: " << currentHandler;
    //
    // wer war das?
    //
    // QWebSocket *nSock = qobject_cast< QWebSocket * >( sender() );
    qDebug() << "commserver: binary msg recived from: " << nSock->peerAddress().toString() << ", " << nSock->peerPort();
    nSock->sendTextMessage( "BINARY ACKNOWLEDGED" );
  }

  /**
   * @brief ConnectionHandler::onDisconnected
   */
  void ConnectionHandler::onDisconnected()
  {
    qDebug() << "ConnectionHandler: in disconnected handler id: " << currentHandler;
    //
    // wer war das?
    //
    // QWebSocket *nSock = qobject_cast< QWebSocket * >( sender() );
    qDebug() << "commserver: closed remote connection from: " << nSock->peerAddress().toString() << ", " << nSock->peerPort();
    emit closed( this );

    // TODO: noch was intelligenten machen
    // remoteSockets.removeAll( nSock );
    // nSock->deleteLater();
  }

}  // namespace bose_commserver
