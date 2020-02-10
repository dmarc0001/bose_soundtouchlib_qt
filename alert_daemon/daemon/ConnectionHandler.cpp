#include "ConnectionHandler.hpp"

#include <QDebug>

namespace bose_commserver
{
  qlonglong ConnectionHandler::handlerNumberCounter{0};

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
      , currentHandlerNum( ConnectionHandler::handlerNumberCounter++ )
      , config( dconfig )
      , connected( false )
      , nSock( theSock )
      , lg( dconfig->getLogger() )
  {
    //
    lg->debug( QString( "ConnectionHandler::ConnectionHandler: created handler id: %1" )
                   .arg( currentHandlerNum, 12, 10, QLatin1Char( '0' ) ) );
    init();
  }

  /**
   * @brief ConnectionHandler::ConnectionHandler
   * @param cp
   */
  ConnectionHandler::ConnectionHandler( const ConnectionHandler &cp ) : currentHandlerNum( cp.currentHandlerNum )
  {
    lg->debug( QString( "ConnectionHandler::ConnectionHandler: copyconstructor created handler id: %1" )
                   .arg( currentHandlerNum, 12, 10, QLatin1Char( '0' ) ) );
    config = cp.config;
    connected = cp.connected;
    nSock = cp.nSock;
    init();
  }

  /**
   * @brief ConnectionHandler::init
   */
  void ConnectionHandler::init()
  {
    lg->debug( "ConnectionHandler::init: connect slots for websocket..." );
    connect( nSock.get(), &QWebSocket::textMessageReceived, this, &ConnectionHandler::onRecTextMessage );
    connect( nSock.get(), &QWebSocket::binaryMessageReceived, this, &ConnectionHandler::onRecBinaryMessage );
    connect( nSock.get(), &QWebSocket::disconnected, this, &ConnectionHandler::onDisconnected );
  }

  /**
   * @brief ConnectionHandler::getNSock
   * @return
   */
  std::shared_ptr< QWebSocket > ConnectionHandler::getNSock() const
  {
    return nSock;
  }

  /**
   * @brief ConnectionHandler::~ConnectionHandler
   */
  ConnectionHandler::~ConnectionHandler()
  {
    lg->debug( QString( "ConnectionHandler::~ConnectionHandler: destruction handler id: %1" )
                   .arg( currentHandlerNum, 12, 10, QLatin1Char( '0' ) ) );
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
  qlonglong ConnectionHandler::getCurrentHandlerNum() const
  {
    return currentHandlerNum;
  }

  /**
   * @brief ConnectionHandler::disconnectWebsocket
   */
  void ConnectionHandler::disconnectWebsocket()
  {
    lg->debug( QString( "ConnectionHandler::disconnectWebsocket: disconnect handler id: %1" )
                   .arg( currentHandlerNum, 12, 10, QLatin1Char( '0' ) ) );
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
    lg->debug( QString( "ConnectionHandler::onRecTextMessage: handler id: %1 recived: %2 from %3" )
                   .arg( currentHandlerNum, 12, 10, QLatin1Char( '0' ) )
                   .arg( msg )
                   .arg( nSock->peerAddress().toString() ) );
    nSock->sendTextMessage( "TEXT ACKNOWLEDGED" );
  }

  /**
   * @brief ConnectionHandler::onRecBinaryMessage
   */
  void ConnectionHandler::onRecBinaryMessage( QByteArray msg )
  {
    lg->debug( QString( "ConnectionHandler::onRecTextMessage: handler id: %1 recived: %2 from %3" )
                   .arg( currentHandlerNum, 12, 10, QLatin1Char( '0' ) )
                   .arg( QString( msg.toHex( ':' ) ) )
                   .arg( nSock->peerAddress().toString() ) );
    nSock->sendTextMessage( "BINARY ACKNOWLEDGED" );
  }

  /**
   * @brief ConnectionHandler::onDisconnected
   */
  void ConnectionHandler::onDisconnected()
  {
    qDebug() << "ConnectionHandler: in disconnected handler id: " << currentHandlerNum;
    //
    // wer war das?
    //
    qDebug() << "commserver: closed remote connection from: " << nSock->peerAddress().toString() << ", " << nSock->peerPort();
    //
    // gib bei der App bescheid, dass alles erledigt ist
    // => Programm beenden
    //
    emit closed( this );
  }

}  // namespace bose_commserver
