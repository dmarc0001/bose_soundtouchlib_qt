#ifndef BOSECOMMSERVER_HPP
#define BOSECOMMSERVER_HPP

#include <signal.h>
#include <QDir>
#include <QException>
#include <QList>
#include <QMutex>
#include <QMutexLocker>
#include <QSocketNotifier>
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include <memory>

#include "config/alertconfig.hpp"
#include "connectionhandler.hpp"
#include "daemontimer.hpp"
#include "logging/logger.hpp"

namespace bose_commserver
{
  class BoseCommServer : public QObject
  {
    Q_OBJECT

    private:
    std::shared_ptr< AlertAppConfig > config;  //! geteilter Zeiger auf Config Objekt
    // std::unique_ptr< QWebSocketServer > cServer;  //! Kommandoserver
    std::shared_ptr< QWebSocketServer > cServer;
    QList< std::shared_ptr< ConnectionHandler > > remoteConnections;  //! Liste mit verbundenen Sockets
    QWebSocket m_webSocket;                                           //! Client Socket zum BOSE Gerät
    std::shared_ptr< Logger > lg;                                     //! wenn ein logger erstellt wurde
    QMutex qMutex;                                                    //! nur einem Zugang gewähren
    std::unique_ptr< DaemonTimer > dTimer;                            //! der Timer für alles
    // static int sighupFd[ 2 ];
    static int sigtermFd[ 2 ];
    // QSocketNotifier *snHup;
    std::unique_ptr< QSocketNotifier > snTerm;

    public:
    explicit BoseCommServer( std::shared_ptr< AlertAppConfig > dconfig, QObject *parent = nullptr );  //! der Konstruktor
    ~BoseCommServer();                                                                                //! Zerstörer
    // Unix signal handlers.
    // static void hupSignalHandler(int unused);
    static void termSignalHandler( int unused );
    void reciveAsyncSignal( int signal );                                                             //! empfange ein Systemsignal

    private:
    bool configServer();  //! erzeuge Einstellungen zum Kommandoserver
    bool createLogger();  //! erzeuge den Logger für den daemon

    signals:
    void closed();  //! signalisiere alles geschlossen -> Ende TODO: brauch ich dann nicht mehr

    public slots:
    // Qt signal handlers.
    // void handleSigHup();
    void onHandleSigTerm();

    private slots:
    void onNewConnection();                                   //! neue ankommende Kommandoverbindung
    void onClosedListening();                                 //! serversocket geschlossen
    void onClientClosed( const ConnectionHandler *handler );  //! wenn ein client die Verbindung beendet hat
    void onStartAlert( SingleAlertConfig &alert );            //! Es soll ein Alarm gestartet werden
    void onStopAlert( SingleAlertConfig &alert );             //! es soll ein alarm gestoppt werden
  };
}  // namespace bose_commserver
#endif  // BOSECOMMSERVER_HPP
