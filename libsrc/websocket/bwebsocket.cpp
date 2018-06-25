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
    req.setRawHeader( QByteArray( "Sec-WebSocket-Protocol" ), QByteArray( "Gabbo" ) );
    webSocket.open( req );
  }

  BWebsocket::~BWebsocket()
  {
    lg->debug( "BWebsocket::~BWebsocket..." );
  }

  void BWebsocket::slotOnWSConnected( void )
  {
    lg->debug( "BWebsocket::slotOnWSConnected..." );
    connect( &webSocket, &QWebSocket::textMessageReceived, this, &BWebsocket::slotOnWSTextMessageReceived );
  }

  void BWebsocket::slotOnWSDisConnected( void )
  {
    lg->debug( QString( "BWebsocket::slotOnWSDisConnected: error <%1>" ).arg( webSocket.errorString() ) );
  }

  void BWebsocket::slotOnWSTextMessageReceived( QString message )
  {
    lg->debug( QString( "BWebsocket::slotOnWSTextMessageReceived: %1" ).arg( message ) );
  }

}  // namespace radio
