#ifndef BWEBSOCKET_HPP
#define BWEBSOCKET_HPP

#include <qglobal.h>
#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <memory>
#include "../logging/Logger.hpp"

namespace radio
{
  class BWebsocket : public QObject
  {
    Q_OBJECT

    private:
    QString hostname;
    qint16 wsPort;
    std::shared_ptr< Logger > lg;
    QWebSocket webSocket;
    QUrl url;

    public:
    explicit BWebsocket( QString &stHost, qint16 stWSPort, std::shared_ptr< Logger > logger, QObject *parent );
    ~BWebsocket();

    private slots:
    void slotOnWSConnected( void );
    void slotOnWSDisConnected( void );
    void slotOnWSTextMessageReceived( QString message );
  };
}  // namespace radio
#endif  // BWEBSOCKET_HPP
