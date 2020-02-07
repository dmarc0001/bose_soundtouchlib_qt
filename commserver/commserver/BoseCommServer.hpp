#ifndef BOSECOMMSERVER_HPP
#define BOSECOMMSERVER_HPP

#include <QDir>
#include <QException>
#include <QList>
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include <memory>

#include "ConnectionHandler.hpp"
#include "config/DaemonConfig.hpp"
#include "logging/Logger.hpp"

namespace bose_commserver
{
  class BoseCommServer : public QObject
  {
    Q_OBJECT

    private:
    std::shared_ptr< DaemonConfig > config;  //! geteilter Zeiger auf Config Objekt
    // std::unique_ptr< QWebSocketServer > cServer;  //! Kommandoserver
    std::shared_ptr< QWebSocketServer > cServer;
    QList< std::shared_ptr< ConnectionHandler > > remoteConnections;  //! Liste mit verbundenen Sockets
    QWebSocket m_webSocket;                                           //! Client Socket zum BOSE Gerät

    public:
    explicit BoseCommServer( std::shared_ptr< DaemonConfig > dconfig, QObject *parent = nullptr );  //! der Konstruktor
    ~BoseCommServer();                                                                              //! Zerstörer

    private:
    bool configServer();  //! erzeuge Einstellungen zum Kommandoserver
    bool createLogger();  //! erzeuge den Logger für den daemon

    signals:
    void closed();  //! signalisiere alles geschlossen -> Ende TODO: brauch ich dann nicht mehr

    private slots:
    void onNewConnection();    //! neue ankommende Kommandoverbindung
    void onClosedListening();  //! serversocket geschlossen
    void onClientClosed( const ConnectionHandler *handler );
    // void remProcTextMessage( QString msg );       //! Kommando als text empfangen
    // void remProcBinaryMessage( QByteArray msg );  //! Kommando binär empfangen
    // void remSocketDisconnected();                 //! Kommandoverbindung beendet
  };
}  // namespace bose_commserver
#endif  // BOSECOMMSERVER_HPP
