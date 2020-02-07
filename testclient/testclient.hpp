#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

#include <QJsonDocument>
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
//

namespace testclient
{
  class TestClient : public QObject
  {
    Q_OBJECT
    public:
    explicit TestClient( const QUrl &url, QObject *parent = nullptr );

    Q_SIGNALS:
    void closed();

    private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived( QString message );
    void onBinaryMessageReceived( QByteArray message );
    void onSocketDisconnected();

    private:
    QWebSocket m_webSocket;
    QUrl m_url;
  };
}  // namespace testclient

#endif  // ECHOCLIENT_H
