#ifndef BWEBSOCKET_HPP
#define BWEBSOCKET_HPP

#include <qglobal.h>
#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <memory>
#include "../logging/Logger.hpp"

namespace bose_soundtoch_lib
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
    void open( void );

    signals:
    void sigOnWSConnected( void );
    void sigOnWSDisConnected( void );
    void sigOnWSTextMessageReceived( QString message );

    private slots:
    void slotOnWSConnected( void );
    void slotOnWSDisConnected( void );
    void slotOnWSTextMessageReceived( QString message );
  };
}  // namespace radio
#endif  // BWEBSOCKET_HPP
