#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>

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

  private:
  QWebSocket m_webSocket;
  QUrl m_url;
};

#endif  // ECHOCLIENT_H
