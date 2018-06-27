#include "bwebsocket.hpp"

namespace bose_soundtoch_lib
{
  BWebsocket::BWebsocket( QString &stHost, qint16 stWSPort, std::shared_ptr< Logger > logger, QObject *parent )
      : QObject( parent ), hostname( stHost ), wsPort( stWSPort ), lg( logger )
  {
    lg->debug( "BWebsocket::BWebsocket..." );
    //
    // erstelle eine URL zum verbinden
    //
    url = QUrl( QString( "ws://%1:%2" ).arg( hostname ).arg( wsPort ) );
  }

  BWebsocket::~BWebsocket()
  {
    lg->debug( "BWebsocket::~BWebsocket..." );
    webSocket.flush();
    webSocket.close();
  }

  void BWebsocket::open( void )
  {
    //
    // verbinde die Signale des Websocket mit den Slots
    //
    connect( &webSocket, &QWebSocket::connected, this, &BWebsocket::slotOnWSConnected );
    connect( &webSocket, &QWebSocket::disconnected, this, &BWebsocket::slotOnWSDisConnected );
    //
    // öffne den Socket
    //
    lg->debug( QString( "BWebsocket::open: open url: %1..." ).arg( url.toString() ) );
    QNetworkRequest req( url );
    req.setRawHeader( QByteArray( "Sec-WebSocket-Version" ), QByteArray( "13" ) );
    req.setRawHeader( QByteArray( "Sec-WebSocket-Protocol" ), QByteArray( "gabbo" ) );
    webSocket.open( req );
  }

  void BWebsocket::slotOnWSConnected( void )
  {
    lg->debug( "BWebsocket::slotOnWSConnected..." );
    connect( &webSocket, &QWebSocket::textMessageReceived, this, &BWebsocket::slotOnWSTextMessageReceived );
    emit sigOnWSConnected();
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
    emit sigOnWSDisConnected();
  }

  void BWebsocket::slotOnWSTextMessageReceived( QString message )
  {
    lg->debug( QString( "BWebsocket::slotOnWSTextMessageReceived: %1" ).arg( message ) );
    emit sigOnWSTextMessageReceived( message );
  }

}  // namespace radio
