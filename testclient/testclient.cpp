#include "testclient.hpp"

#include <QtCore/QDebug>

namespace testclient
{
  TestClient::TestClient( const QUrl &url, QObject *parent ) : QObject( parent ), m_url( url )
  {
    qDebug() << "WebSocket server:" << m_url.toString();
    connect( &m_webSocket, &QWebSocket::connected, this, &TestClient::onConnected );
    connect( &m_webSocket, &QWebSocket::disconnected, this, &TestClient::closed );
    m_webSocket.open( QUrl( url ) );
  }

  void TestClient::onConnected()
  {
    qDebug() << "WebSocket connected...";
    connect( &m_webSocket, &QWebSocket::textMessageReceived, this, &TestClient::onTextMessageReceived );
    connect( &m_webSocket, &QWebSocket::binaryMessageReceived, this, &TestClient::onBinaryMessageReceived );
    connect( &m_webSocket, &QWebSocket::disconnected, this, &TestClient::onSocketDisconnected );
    qDebug() << "WebSocket send message...";
    m_webSocket.sendTextMessage( QStringLiteral( "Hello, world!" ) );
  }

  void TestClient::onTextMessageReceived( QString message )
  {
    qDebug() << "Message received:" << message;
    m_webSocket.close();
  }

  void TestClient::onBinaryMessageReceived( QByteArray message )
  {
    qDebug() << "Binary Message received:" << message.toHex( ':' );
  }

  void TestClient::onSocketDisconnected()
  {
    qDebug() << "Server was disconnected!";
  }
}  // namespace testclient
