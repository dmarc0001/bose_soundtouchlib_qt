#include "testclient.hpp"

#include <QtCore/QDebug>

QT_USE_NAMESPACE

TestClient::TestClient( const QUrl &url, QObject *parent ) : QObject( parent ), m_url( url )
{
  qDebug() << "WebSocket server:" << url;
  connect( &m_webSocket, &QWebSocket::connected, this, &TestClient::onConnected );
  connect( &m_webSocket, &QWebSocket::disconnected, this, &TestClient::closed );
  m_webSocket.open( QUrl( url ) );
}

void TestClient::onConnected()
{
  qDebug() << "WebSocket connected";
  connect( &m_webSocket, &QWebSocket::textMessageReceived, this, &TestClient::onTextMessageReceived );
  m_webSocket.sendTextMessage( QStringLiteral( "Hello, world!" ) );
}

void TestClient::onTextMessageReceived( QString message )
{
  qDebug() << "Message received:" << message;
  m_webSocket.close();
}
