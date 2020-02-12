#ifndef CONNECTIONHANDLER_HPP
#define CONNECTIONHANDLER_HPP

#include <QJsonDocument>
#include <QLatin1Char>
#include <QObject>
#include <QWebSocket>
#include <memory>

#include "config/alertconfig.hpp"

namespace bose_commserver
{
  class ConnectionHandler : public QObject
  {
    Q_OBJECT
    private:
    static qlonglong handlerNumberCounter;
    const qlonglong currentHandlerNum;
    std::shared_ptr< AlertAppConfig > config;
    bool connected;
    std::shared_ptr< QWebSocket > nSock;
    std::shared_ptr< Logger > lg;  //! wenn ein logger erstellt wurde

    public:
    explicit ConnectionHandler( std::shared_ptr< AlertAppConfig > dconfig,
                                std::shared_ptr< QWebSocket > theSock,
                                QObject *parent = nullptr );
    ConnectionHandler( const ConnectionHandler &cp );
    void init();
    ~ConnectionHandler();
    void disconnectWebsocket();              //! Verbindung trennen
    qlonglong getCurrentHandlerNum() const;  // den aktuellen handler identifizieren
    std::shared_ptr< QWebSocket > getNSock() const;

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
