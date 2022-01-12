#ifndef BOSECOMMSERVER_HPP
#define BOSECOMMSERVER_HPP

#include <signal.h>

#include <QDir>
#include <QException>
#include <QList>
#include <QMutex>
#include <QMutexLocker>
#include <QSocketNotifier>
#include <QThread>
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include <memory>
#include "bosesoundalert.hpp"
#include "config/alertconfig.hpp"
#include "config/common_def.hpp"
#include "connectionhandler.hpp"
#include "daemontimer.hpp"
#include "discover/soundtouchdiscover.hpp"
#include "logging/logger.hpp"
#include "support/nologgerexception.hpp"

namespace bose_commserver
{
  class BoseCommServer : public QObject
  {
    Q_OBJECT

    private:
    std::shared_ptr< AlertAppConfig > config;          //! geteilter Zeiger auf Config Objekt
    std::shared_ptr< QWebSocketServer > cServer;       //! der Serversocket
    std::unique_ptr< SoundTouchDiscover > stDiscover;  //! Findet Soundtouch Devices
    ConnectionHandlerList remoteConnections;           //! Liste mit verbundenen Sockets
    QWebSocket m_webSocket;                            //! Client Socket zum BOSE Gerät
    LoggerPtr lg;                                      //! wenn ein logger erstellt wurde
    QMutex qMutex;                                     //! nur einem Zugang gewähren
    std::unique_ptr< DaemonTimer > dTimer;             //! der Timer für alles
    AlertTaskList activeAlerts;                        //! Liste mit aktiven alerts *BoseSoundAlert
    public:
    explicit BoseCommServer( std::shared_ptr< AlertAppConfig > dconfig, QObject *parent = nullptr );  //! der Konstruktor
    ~BoseCommServer();                                                                                //! Zerstörer
    void reciveSignal( int signal );                                                                  //! empfange ein Systemsignal

    private:
    bool configServer();  //! erzeuge Einstellungen zum Kommandoserver
    bool createLogger();  //! erzeuge den Logger für den daemon

    signals:
    void closed();  //! signalisiere alles geschlossen -> Ende TODO: brauch ich dann nicht mehr

    public slots:

    private slots:
    void onNewConnection();                                                    //! neue ankommende Kommandoverbindung
    void onClosedListening();                                                  //! serversocket geschlossen
    void onClientClosed( const bose_commserver::ConnectionHandler *handler );  //! wenn ein client die Verbindung beendet hat
    void onStartAlert( bose_commserver::SingleAlertConfig &alert );            //! Es soll ein Alarm gestartet werden
    void onStopAlert( bose_commserver::SingleAlertConfig &alert );             //! es soll ein alarm gestoppt werden
    void onAlertFinish( bose_commserver::BoseSoundAlert *alert );              //! wenn ein Alarm sich beendet hat
  };
}  // namespace bose_commserver
#endif  // BOSECOMMSERVER_HPP
