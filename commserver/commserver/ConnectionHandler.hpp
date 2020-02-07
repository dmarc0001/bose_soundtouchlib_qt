#ifndef CONNECTIONHANDLER_HPP
#define CONNECTIONHANDLER_HPP

#include <QJsonDocument>
#include <QObject>
#include <QWebSocket>
#include <memory>

#include "config/DaemonConfig.hpp"

namespace bose_commserver
{
  class ConnectionHandler : public QObject
  {
    Q_OBJECT
    private:
    static ulong handlerNumberCounter;
    const ulong currentHandler;
    std::shared_ptr< DaemonConfig > config;
    bool connected;
    std::shared_ptr< QWebSocket > nSock;

    public:
    explicit ConnectionHandler( std::shared_ptr< DaemonConfig > dconfig,
                                std::shared_ptr< QWebSocket > theSock,
                                QObject *parent = nullptr );
    ConnectionHandler( const ConnectionHandler &cp );
    void init();
    ~ConnectionHandler();
    void disconnectWebsocket();       //! Verbindung trennen
    ulong getCurrentHandler() const;  // den aktuellen handler identifizieren

    signals:
    void closed( const ConnectionHandler *handler );

    // private slots:
    public slots:
    void onRecTextMessage( QString msg );       //! Kommando als text empfangen
    void onRecBinaryMessage( QByteArray msg );  //! Kommando binär empfangen
    void onDisconnected();                      //! Kommandoverbindung beendet
  };
}  // namespace bose_commserver
#endif  // CONNECTIONHANDLER_HPP
