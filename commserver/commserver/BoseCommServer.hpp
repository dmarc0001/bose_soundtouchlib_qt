#ifndef BOSECOMMSERVER_HPP
#define BOSECOMMSERVER_HPP

#include <QException>
#include <QList>
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include <memory>
#include "config/DaemonConfig.hpp"

namespace bose_commserver
{
  class BoseCommServer : public QObject
  {
    Q_OBJECT

    private:
    std::shared_ptr< DaemonConfig > config;       //! geteilter Zeiger auf Config Objekt
    std::unique_ptr< QWebSocketServer > cServer;  //! Kommandoserver
    QList< QWebSocket * > remoteSockets;          //! Liste mit verbundenen Sockets, benötigt für Meldungenzu den clienten
    QWebSocket m_webSocket;                       //! Client Socket zum BOSE Gerät

    public:
    explicit BoseCommServer( std::shared_ptr< DaemonConfig > dconfig, QObject *parent = nullptr );  //! der Konstruktor
    ~BoseCommServer();                                                                              //! Zerstörer

    private:
    bool createServer();  //! erzeuge Einstellungen zum Kommandoserver

    Q_SIGNALS:
    void closed();  //! signalisiere alles geschlossen -> Ende TODO: brauch ich dann nicht mehr

    private Q_SLOTS:
    void newRemConnection();                      //! neue ankommende Kommandoverbindung
    void closedRemListening();                    //! serversocket geschlossen
    void remProcTextMessage( QString msg );       //! Kommando als text empfangen
    void remProcBinaryMessage( QByteArray msg );  //! Kommando binär empfangen
    void remSocketDisconnected();                 //! Kommandoverbindung beendet
  };
}  // namespace bose_commserver
#endif  // BOSECOMMSERVER_HPP
