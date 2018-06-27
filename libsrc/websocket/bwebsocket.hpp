#ifndef BWEBSOCKET_HPP
#define BWEBSOCKET_HPP

#include <qglobal.h>
#include <QObject>
#include <QtDebug>
#include <QtWebSockets/QWebSocket>
#include <memory>

namespace bose_soundtoch_lib
{
  class BWebsocket : public QObject
  {
    Q_OBJECT

    private:
    QString hostname;
    qint16 wsPort;
    QWebSocket webSocket;
    QUrl url;

    public:
    explicit BWebsocket( QString &stHost, qint16 stWSPort, QObject *parent );
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
}  // namespace bose_soundtoch_lib
#endif  // BWEBSOCKET_HPP
