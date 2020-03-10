#include "bwebsocket.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief BWebsocket::BWebsocket
   * @param stHost
   * @param stWSPort
   * @param parent
   */
  BWebsocket::BWebsocket( QString &stHost, quint16 stWSPort, QObject *parent )
      : QObject( parent ), hostname( stHost ), wsPort( stWSPort )
  {
    qDebug() << "...";
    //
    // erstelle eine URL zum verbinden
    //
    url = QUrl( QString( "ws://%1:%2" ).arg( hostname ).arg( wsPort ) );
  }

  /**
   * @brief BWebsocket::~BWebsocket
   */
  BWebsocket::~BWebsocket()
  {
    qDebug() << "...";
    close();
  }

  /**
   * @brief BWebsocket::open
   */
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

  /**
   * @brief BWebsocket::isValid
   * @return
   */
  bool BWebsocket::isValid( void )
  {
    return ( webSocket.isValid() );
  }
  /**
   * @brief BWebsocket::close
   */
  void BWebsocket::close( void )
  {
    qDebug() << "...";
    if ( webSocket.isValid() )
    {
      webSocket.flush();
      webSocket.close();
      // webSocket.abort();
    }
  }

  /**
   * @brief BWebsocket::slotOnWSConnected
   */
  void BWebsocket::slotOnWSConnected( void )
  {
    qDebug() << "...";
    connect( &webSocket, &QWebSocket::textMessageReceived, this, &BWebsocket::slotOnWSTextMessageReceived );
    emit sigOnWSConnected();
  }

  /**
   * @brief BWebsocket::slotOnWSDisConnected
   */
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

  /**
   * @brief BWebsocket::slotOnWSTextMessageReceived
   * @param message
   */
  void BWebsocket::slotOnWSTextMessageReceived( QString message )
  {
    qDebug() << "-------------------------------------------------------------------";
    qDebug() << message;
    qDebug() << "-------------------------------------------------------------------";
    emit sigOnWSTextMessageReceived( message );
  }

}  // namespace bose_soundtoch_lib
