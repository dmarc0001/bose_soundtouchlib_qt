//
// oberstes level JSON: "get", "set", "delete" ARRAY
//
// ARRAY besteht aus Objekten
//
// get Kommando : config-id,config,all,devices
//                config-id  : Version der config
//                config     : komplette Konfiguration
//                alert      : einen oder merhrere alarme, oder alle
//                devices    : verfügbare geräte
//                new        : neue, leere Konfiguration für einen Alarm
//
// set Kommando: alert
//               alle eigenschaften eines Alarmes
//               name, enable, preset|source
//
// delete Kommando:
//               alert (alert-xx)
//
// propertys für alert
//               volume, date, location, source, raise_vol, duration, source_account,
//               days, devices, type, enable, note, time
//
// {"delete":[{"alert":"alert-04"}, {"alert":"alert-03"}]}
// {"set":[{"alert":"alert-04","enable":"true", ...}, {"alert":"alert-03","enable":"true", ...}]}
//
#include "testclient.hpp"
#include <QtCore/QDebug>

namespace testclient
{
  TestClient::TestClient( const QUrl &url, QObject *parent ) : QObject( parent ), m_url( url ), counter( 0 )
  {
    qDebug() << "WebSocket server:" << m_url.toString();
    connect( &m_webSocket, &QWebSocket::connected, this, &TestClient::onConnected );
    connect( &m_webSocket, &QWebSocket::disconnected, this, &TestClient::closed );
    m_webSocket.open( QUrl( url ) );
    timer.setInterval( 800 );
  }

  void TestClient::onConnected()
  {
    qDebug() << "WebSocket connected...";
    connect( &timer, &QTimer::timeout, this, &TestClient::onTimer );
    timer.start();
    connect( &m_webSocket, &QWebSocket::textMessageReceived, this, &TestClient::onTextMessageReceived );
    connect( &m_webSocket, &QWebSocket::binaryMessageReceived, this, &TestClient::onBinaryMessageReceived );
    connect( &m_webSocket, &QWebSocket::disconnected, this, &TestClient::onSocketDisconnected );
    qDebug() << "WebSocket send message...";
    m_webSocket.sendTextMessage( QStringLiteral( "Hello, world!" ) );
  }

  void TestClient::onTextMessageReceived( const QString &message )
  {
    qDebug() << "Message received:" << message;
    // m_webSocket.close();
  }

  void TestClient::onBinaryMessageReceived( const QByteArray &message )
  {
    qDebug() << "Binary Message received:" << message.toHex( ':' );
  }

  void TestClient::onSocketDisconnected()
  {
    qDebug() << "Server was disconnected!";
  }

  void TestClient::onTimer()
  {
    if ( counter < 5 )
    {
      std::shared_ptr< QJsonObject > firstObj = getFirstJSONObject();
      QJsonDocument doc( *firstObj );
      // QString strJson( doc.toJson( QJsonDocument::Indented ) );
      QString strJson( doc.toJson( QJsonDocument::Compact ) );
      counter++;
      m_webSocket.sendTextMessage( strJson );
    }
    else if ( counter < 60 )
    {
      counter++;
      m_webSocket.sendTextMessage( QString( "socket counter: - %1 -" ).arg( counter, 4, 10, QLatin1Char( '0' ) ) );
    }
    else
    {
      m_webSocket.close();
    }
  }

  /**
   * @brief getFirstJSONObject
   * @return
   */
  std::shared_ptr< QJsonObject > TestClient::getFirstJSONObject()
  {
    std::shared_ptr< QJsonObject > testObj = std::shared_ptr< QJsonObject >( new QJsonObject() );
    QJsonObject deviceProps_01;
    deviceProps_01.insert( "name", QLatin1String( "device 01" ) );
    deviceProps_01.insert( "volume", QLatin1String( "23" ) );
    deviceProps_01.insert( "preset", QLatin1String( "4" ) );
    QJsonObject deviceProps_02;
    deviceProps_02.insert( "name", QLatin1String( "device 02" ) );
    deviceProps_02.insert( "volume", QLatin1String( "12" ) );
    deviceProps_02.insert( "preset", QLatin1String( "1" ) );
    //
    QJsonArray devices;
    devices.append( deviceProps_01 );
    devices.append( deviceProps_02 );
    //
    testObj->insert( "props", devices );
    //
    return ( testObj );
  }

}  // namespace testclient
