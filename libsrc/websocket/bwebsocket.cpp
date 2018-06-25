#include "bwebsocket.hpp"

namespace radio
{
  BWebsocket::BWebsocket( QString &stHost, qint16 stWSPort, std::shared_ptr< Logger > logger, QObject *parent )
      : QObject( parent ), hostname( stHost ), wsPort( stWSPort ), lg( logger )
  {
    lg->debug( "BWebsocket::BWebsocket..." );
    //
    // erstelle eine URL zum verbinden
    //
    url = QUrl( QString( "ws://%1:%2" ).arg( hostname ).arg( wsPort ) );
    // url = QUrl( QString( "ws://%1:8080" ).arg( hostname ).arg( wsPort ) );
    //
    // verbinde die Signale des Websocket mit den Slots
    //
    connect( &webSocket, &QWebSocket::connected, this, &BWebsocket::slotOnWSConnected );
    connect( &webSocket, &QWebSocket::disconnected, this, &BWebsocket::slotOnWSDisConnected );
    //
    // öffne den Socket
    //
    lg->debug( QString( "BWebsocket::BWebsocket: open url: %1..." ).arg( url.toString() ) );
    QNetworkRequest req( url );
    req.setRawHeader( QByteArray( "Sec-WebSocket-Version" ), QByteArray( "13" ) );
    req.setRawHeader( QByteArray( "Sec-WebSocket-Protocol" ), QByteArray( "gabbo" ) );
    webSocket.open( req );
  }

  BWebsocket::~BWebsocket()
  {
    lg->debug( "BWebsocket::~BWebsocket..." );
    webSocket.flush();
    webSocket.close();
  }

  void BWebsocket::slotOnWSConnected( void )
  {
    lg->debug( "BWebsocket::slotOnWSConnected..." );
    connect( &webSocket, &QWebSocket::textMessageReceived, this, &BWebsocket::slotOnWSTextMessageReceived );
  }

  void BWebsocket::slotOnWSDisConnected( void )
  {
    if ( webSocket.closeCode() == QWebSocketProtocol::CloseCodeNormal )
    {
      lg->debug( "BWebsocket::slotOnWSDisConnected: normal disconnected" );
    }
    else
    {
      lg->debug( QString( "BWebsocket::slotOnWSDisConnected: reson: <%1> error <%2>" )
                     .arg( webSocket.closeReason() )
                     .arg( webSocket.errorString() ) );
    }
  }

  void BWebsocket::slotOnWSTextMessageReceived( QString message )
  {
    lg->debug( QString( "BWebsocket::slotOnWSTextMessageReceived: %1" ).arg( message ) );
  }

}  // namespace radio
