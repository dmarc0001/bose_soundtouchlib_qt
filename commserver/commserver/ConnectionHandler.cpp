#include "ConnectionHandler.hpp"

#include <QDebug>

namespace bose_commserver
{
  ConnectionHandler::ConnectionHandler( std::shared_ptr< DaemonConfig > dconfig,
                                        std::shared_ptr< QWebSocket > theSock,
                                        QObject *parent )
      : QObject( parent ), config( dconfig ), connected( false ), nSock( theSock )
  {
  }

  ConnectionHandler::ConnectionHandler( const ConnectionHandler &cp )
  {
    config = cp.config;
    connected = cp.connected;
    nSock = cp.nSock;
  }

  ConnectionHandler::~ConnectionHandler()
  {
    // Websocket verbindung beenden
    if ( nSock->isValid() )
      if ( connected )
      {
        nSock->close();
        connected = false;
      }
    nSock->deleteLater();
  }

  void ConnectionHandler::disconnectWebsocket()
  {
    // Websocket verbindung beenden
    if ( nSock->isValid() )
      if ( connected )
      {
        nSock->close();
      }
    connected = false;
    nSock = nullptr;
  }

  void ConnectionHandler::remProcTextMessage( QString msg )
  {
    //
    // wer war das?
    //
    // QWebSocket *nSock = qobject_cast< QWebSocket * >( sender() );
    qDebug() << "commserver: msg recived: " << msg << " from: " << nSock->peerAddress().toString() << ", " << nSock->peerPort();
    nSock->sendTextMessage( msg );
  }

  void ConnectionHandler::remProcBinaryMessage( QByteArray )
  {
    //
    // wer war das?
    //
    // QWebSocket *nSock = qobject_cast< QWebSocket * >( sender() );
    qDebug() << "commserver: binary msg recived from: " << nSock->peerAddress().toString() << ", " << nSock->peerPort();
  }

  void ConnectionHandler::remSocketDisconnected()
  {
    //
    // wer war das?
    //
    // QWebSocket *nSock = qobject_cast< QWebSocket * >( sender() );
    qDebug() << "commserver: closed remote connection from: " << nSock->peerAddress().toString() << ", " << nSock->peerPort();
    // TODO: noch was intelligenten machen
    // remoteSockets.removeAll( nSock );
    // nSock->deleteLater();
  }

}  // namespace bose_commserver
