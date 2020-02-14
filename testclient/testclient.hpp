#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <memory>
//

namespace testclient
{
  enum LogLevel : quint8
  {
    LG_NONE = 0,
    LG_CRIT,  // 1
    LG_WARN,  // 2
    LG_INFO,  // 3
    LG_DEBUG  // 4
  };

  using JsonObjSPtr = std::shared_ptr< QJsonObject >;

  class TestClient : public QObject
  {
    Q_OBJECT
    private:
    const static char *names[];
    QWebSocket m_webSocket;
    QUrl m_url;
    int counter;
    QTimer timer;

    public:
    explicit TestClient( const QUrl &url, QObject *parent = nullptr );

    private:
    JsonObjSPtr getSetLoglevelJSONObject( const LogLevel level );
    JsonObjSPtr getFirstJSONObject();
    // std::shared_ptr< QJsonObject > getDeviceVolumeJSONObject( const QString device, qint8 volume );

    signals:
    void closed();

    private slots:
    void onConnected();
    void onTextMessageReceived( const QString &message );
    void onBinaryMessageReceived( const QByteArray &message );
    void onSocketDisconnected();
    void onTimer();
  };
}  // namespace testclient

#endif  // ECHOCLIENT_H
