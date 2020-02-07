#ifndef CONNECTIONHANDLER_HPP
#define CONNECTIONHANDLER_HPP

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
    std::shared_ptr< DaemonConfig > config;
    bool connected;
    std::shared_ptr< QWebSocket > nSock;

    public:
    explicit ConnectionHandler( std::shared_ptr< DaemonConfig > dconfig,
                                std::shared_ptr< QWebSocket > theSock,
                                QObject *parent = nullptr );
    ConnectionHandler( const ConnectionHandler &cp );
    ~ConnectionHandler();
    void disconnectWebsocket();  //! Verbindung trennen

    signals:

    // private slots:
    public slots:
    void remProcTextMessage( QString msg );       //! Kommando als text empfangen
    void remProcBinaryMessage( QByteArray msg );  //! Kommando binär empfangen
    void remSocketDisconnected();                 //! Kommandoverbindung beendet
  };
}  // namespace bose_commserver
#endif  // CONNECTIONHANDLER_HPP
