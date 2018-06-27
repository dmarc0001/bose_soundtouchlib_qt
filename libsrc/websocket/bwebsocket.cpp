#include "bwebsocket.hpp"

namespace bose_soundtoch_lib
{
  BWebsocket::BWebsocket( QString &stHost, qint16 stWSPort, QObject *parent )
      : QObject( parent ), hostname( stHost ), wsPort( stWSPort )
  {
    qDebug() << "...";
    //
    // erstelle eine URL zum verbinden
    //
    url = QUrl( QString( "ws://%1:%2" ).arg( hostname ).arg( wsPort ) );
  }

  BWebsocket::~BWebsocket()
  {
    qDebug() << "...";
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
    qDebug() << "open url: " << url.toString();
    QNetworkRequest req( url );
    req.setRawHeader( QByteArray( "Sec-WebSocket-Version" ), QByteArray( "13" ) );
    req.setRawHeader( QByteArray( "Sec-WebSocket-Protocol" ), QByteArray( "gabbo" ) );
    webSocket.open( req );
  }

  void BWebsocket::slotOnWSConnected( void )
  {
    qDebug() << "...";
    connect( &webSocket, &QWebSocket::textMessageReceived, this, &BWebsocket::slotOnWSTextMessageReceived );
    emit sigOnWSConnected();
  }

  void BWebsocket::slotOnWSDisConnected( void )
  {
    if ( webSocket.closeCode() == QWebSocketProtocol::CloseCodeNormal )
    {
      qDebug() << "normal disconnected";
    }
    else
    {
      qWarning() << "reson: " << webSocket.closeReason() << " error: " << webSocket.errorString();
    }
    emit sigOnWSDisConnected();
  }

  void BWebsocket::slotOnWSTextMessageReceived( QString message )
  {
    qDebug() << message;
    emit sigOnWSTextMessageReceived( message );
  }

}  // namespace bose_soundtoch_lib
