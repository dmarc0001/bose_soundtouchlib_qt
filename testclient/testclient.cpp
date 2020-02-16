/******************************************************************************
  oberstes level JSON: "config", "get", "set", "delete" ARRAY

  ARRAY besteht aus Objekten


  get Kommando :
                config-id,config(,all,devices)
                config-id  : Version der config
                config     : komplette Konfiguration
                alert      : einen oder merhrere alarme, oder alle
                devices    : verfügbare geräte
                new        : neue, leere Konfiguration für einen Alarm

  set Kommando: alert       :
                            alle eigenschaften eines Alarmes
                            name, enable, preset|source
               loglevel    : loglevel(0..4)

  delete Kommando:
               alert (alert-xx)

  propertys für alert
               volume, date, location, source, raise_vol, duration, source_account,
               days, devices, type, enable, note, time

  {
    "get":
      {
        "config":"all"    (alarms) oder
        "config":"devices"  (geräte) oder
        "config","daemon"  (app)
      }
  }
  {
    "get":
      {
        "alert-01",
        "alert-03"
      }
  }
  {
    "delete":
      {
        "alert":"alert-04",
        "alert":"alert-03"
      }
  }
  {
    "set":
      {
        "alert-04":{"enable":"true", ...},
        "alert-03":{"enable":"true", ...}
      }
  }
  ############## antworten ##################
  config, error, ack

  CONFIG Antwort
  {
     "xxxx":{},
     "yyyy": {}
  }

  {
    "error": {},
    "error": {}....
  }

  {
    "ack": {},
    "ack": {}....
  }
******************************************************************************/

#include "testclient.hpp"

#include <QtCore/QDebug>

namespace testclient
{
  const char *TestClient::names[] = {"NONE", "CRIT", "WARN", "INFO", "DEBUG"};

  /**
   * @brief TestClient::TestClient
   * @param url
   * @param parent
   */
  TestClient::TestClient( const QUrl &url, QObject *parent ) : QObject( parent ), m_url( url ), counter( 0 )
  {
    qDebug() << "WebSocket server:" << m_url.toString();
    connect( &m_webSocket, &QWebSocket::connected, this, &TestClient::onConnected );
    connect( &m_webSocket, &QWebSocket::disconnected, this, &TestClient::closed );
    m_webSocket.open( QUrl( url ) );
    timer.setInterval( 800 );
  }

  /**
   * @brief TestClient::onConnected
   */
  void TestClient::onConnected()
  {
    qDebug() << "WebSocket connected...";
    connect( &timer, &QTimer::timeout, this, &TestClient::onTimer );
    timer.start();
    connect( &m_webSocket, &QWebSocket::textMessageReceived, this, &TestClient::onTextMessageReceived );
    connect( &m_webSocket, &QWebSocket::binaryMessageReceived, this, &TestClient::onBinaryMessageReceived );
    connect( &m_webSocket, &QWebSocket::disconnected, this, &TestClient::onSocketDisconnected );
    qDebug() << "WebSocket send message...";
    // m_webSocket.sendTextMessage( QStringLiteral( "Hello, world!" ) );
  }

  /**
   * @brief TestClient::onTextMessageReceived
   * @param message
   */
  void TestClient::onTextMessageReceived( const QString &message )
  {
    qDebug() << "Message received:" << message;
    // m_webSocket.close();
  }

  /**
   * @brief TestClient::onBinaryMessageReceived
   * @param message
   */
  void TestClient::onBinaryMessageReceived( const QByteArray &message )
  {
    qDebug() << "Binary Message received:" << message.toHex( ':' );
  }

  /**
   * @brief TestClient::onSocketDisconnected
   */
  void TestClient::onSocketDisconnected()
  {
    qDebug() << "Server was disconnected!";
  }

  /**
   * @brief TestClient::onTimer
   */
  void TestClient::onTimer()
  {
    if ( counter < 20 )
    {
      switch ( counter )
      {
        case 0:
          // debug umschalten
          // qDebug() << "send set loglevel to debug";
          // m_webSocket.sendTextMessage( *setLoglevelJSONObject( LogLevel::LG_DEBUG ) );
          break;
        case 1:
          // dämon konfig anfordern
          qDebug() << "request daemon config";
          m_webSocket.sendTextMessage( *getDaemonJSONConfig() );
          break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
          // dämon konfig anfordern
          qDebug() << "request alert" << counter - 1 << "config";
          m_webSocket.sendTextMessage( *getAlertJSONConfig( counter - 1 ) );
          break;
        case 8:
          // alle alarme
          qDebug() << "request all alerts config";
          m_webSocket.sendTextMessage( *getAllAlertJSONConfig() );
          break;
        case 9:
          // debug umschalten
          // qDebug() << "send set loglevel to info";
          // m_webSocket.sendTextMessage( *setLoglevelJSONObject( LogLevel::LG_INFO ) );
          break;
        default:
          qDebug() << "noop...";
      }
      counter++;
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
   * @brief TestClient::setLoglevelJSONObject
   * @param level
   * @return
   */
  JSonStringPtr TestClient::setLoglevelJSONObject( LogLevel level )
  {
    // das Objekt zum Übergeben
    JsonObjSPtr jsonObj = JsonObjSPtr( new QJsonObject() );
    // Das Objekt für SET Commandos
    QJsonObject setObj;
    setObj.insert( "loglevel", TestClient::names[ static_cast< int >( level ) ] );
    jsonObj->insert( "set", setObj );
    QJsonDocument doc( *jsonObj );
    JSonStringPtr strJson = JSonStringPtr( new QString( doc.toJson( QJsonDocument::Indented ) ) );
    return strJson;
  }

  /**
   * @brief TestClient::getDaemonConfig
   * @return
   */
  JSonStringPtr TestClient::getDaemonJSONConfig()
  {
    // das Objekt zum Übergeben
    JsonObjSPtr jsonObj = JsonObjSPtr( new QJsonObject() );
    // Das Array für GET Commandos
    QJsonObject getObj;
    getObj.insert( "config", "daemon" );
    jsonObj->insert( "get", getObj );
    QJsonDocument doc( *jsonObj );
    JSonStringPtr strJson = JSonStringPtr( new QString( doc.toJson( QJsonDocument::Indented ) ) );
    return strJson;
  }

  JSonStringPtr TestClient::getAlertJSONConfig( int which )
  {
    // das Objekt zum Übergeben
    JsonObjSPtr jsonObj = JsonObjSPtr( new QJsonObject() );
    QJsonObject getObj;
    getObj.insert( "config", QString( "alert-%1" ).arg( which, 2, 10, QChar( '0' ) ) );
    jsonObj->insert( "get", getObj );
    QJsonDocument doc( *jsonObj );
    JSonStringPtr strJson = JSonStringPtr( new QString( doc.toJson( QJsonDocument::Indented ) ) );
    return strJson;
  }

  JSonStringPtr TestClient::getAllAlertJSONConfig()
  {
    // das Objekt zum Übergeben
    JsonObjSPtr jsonObj = JsonObjSPtr( new QJsonObject() );
    QJsonObject getObj;
    getObj.insert( "config", "all" );
    jsonObj->insert( "get", getObj );
    QJsonDocument doc( *jsonObj );
    JSonStringPtr strJson = JSonStringPtr( new QString( doc.toJson( QJsonDocument::Indented ) ) );
    return strJson;
  }
}  // namespace testclient
