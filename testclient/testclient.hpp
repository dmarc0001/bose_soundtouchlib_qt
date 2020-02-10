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
  class TestClient : public QObject
  {
    Q_OBJECT
    private:
    QWebSocket m_webSocket;
    QUrl m_url;
    int counter;
    QTimer timer;

    public:
    explicit TestClient( const QUrl &url, QObject *parent = nullptr );

    private:
    std::shared_ptr< QJsonObject > getFirstJSONObject();
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
