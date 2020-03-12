#ifndef BOSESOUNDALERT_HPP
#define BOSESOUNDALERT_HPP

#include <QList>
#include <QMutex>
#include <QObject>
#include <QRegExp>
#include <QThread>
#include <QTimer>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include <bsoundtouchdevice.hpp>
#include <memory>
#include "config/alertconfig.hpp"
#include "config/common_def.hpp"
#include "logging/logger.hpp"
#include "xmlparser/xmlupdateparser.hpp"

namespace bose_commserver
{
  class BoseSoundAlert;

  using AlertTaskList = QList< bose_commserver::BoseSoundAlert * >;
  using sDevicePtr = std::shared_ptr< bose_soundtoch_lib::BSoundTouchDevice >;
  using ResponseObjPtr = std::shared_ptr< bose_soundtoch_lib::IResponseObject >;
  constexpr int timerValue = 100;                                  //! Timer 100 ms
  constexpr quint32 factorTo1Sec = 10;                             //! Faktor für timervalue * factor = 1000 ms
  constexpr quint32 timeoutValue = quint32( 10000 / timerValue );  //! 2.5 Sekunden Timeout

  class BoseSoundAlert : public QThread
  {
    Q_OBJECT
    private:
    enum alertStatusType : quint8  //! Stati des "automaten", des Alarm
    {
      AL_INACTIVE = 0,   // Nagelneuer Status
      AL_PREPARE1,       // Gerät und Objekt vorbereiten
      AL_PREPARE2,       // Gerät auf Anfangseinstellungen STANDBY
      AL_PREPARE3,       // Gerät auf Anfangseinstellungen CURRENT VOLUME
      AL_PREPARE4,       // Gerät auf Anfangseinstellungen VOLUME auf 0
      AL_PREPARE5,       // Gerät auf Anfangseinstellungen TUNER
      AL_PREPARE6,       // Gerät auf Anfangseinstellungen Warte auf Abspielen des Senders
      AL_RAISE,          // Lautstärke erhöhen bis zum Soll
      AL_PLAY,           // Alarm in Betrieb
      AL_REDUCE,         // Lautstärke wieder senken
      AL_POSTPARE,       // Abschlußarbeiten
      AL_FINISHED,       // Alarm hat fertich
      AL_WAITFOR = 0x80  // warte auf eine Antwort
    };
    enum waitForType : quint8  //! auf was warten wir
    {
      WT_VOLUME,     // warte auf Lautstärke
      WT_SOURCE,     // warte auf Senderwechsel
      WT_PLAYSTATE,  // Abspielstatus
      WT_STANDBY     // standby
    };
    enum waitState : quint8
    {
      WS_WAITING,  //! warten
      WS_SUCCESS,  //! warten ohne timeout
      WS_TIMEOUT   //! warten mit Timeout
    };
    static const QRegExp rePreset;     //! Regulärer Ausdruck zum finden der PRESET Source
    static const QRegExp rePresetNum;  //! Regulärer Ausdruck zum finden der PRESET Source Nummer
    static const QRegExp reInet;       //! Regulärer Ausdruck zum finden der INGTERNET Source
    static const QRegExp reAmazon;     //! Regulärer Ausdruck zum finden der AMAZON Source
    static const QRegExp reStandby;    //! Regulärer Ausdruck zum finden des STANDBY Status
    static quint16 alertCount;         //! Anzahl aktiver alarme
    SingleAlertConfig alConfig;        //! Kopie der Alarmkonfiguration
    AppConfigPtr config;               //! Globale Konfiguration
    LoggerPtr lg;                      //! der systemlogger
    quint16 duration;                  //! der Startwert aus der config
    quint32 durationCounter;           //! wenn der auf 0 runter gezählt wird ist ENDE
    int mainTimerId{-1};               //! id des haupttimers
    int alertVolume{0};                //! wenn vol raising dann hier für speed cachen
    int currentVolume{0};              //! wenn vol raising dann hier für speed cachen
    QString currentSource{""};         //! was spielt die Kiste gerade
    QString currentPlaystate{""};      //! welcher Status? (PLAYING, BUFFERING, STANDBY)
    int bevorVolume{0};                //! Lautstärke vor dem Alarm
    int alertRaiseStep{1};             //! wie viel soll die Lautstärke per Tick erhöht werden?
    QList< sDevicePtr > devices;       //! Geräte im Alarm, die Instanzen löschen sich selber
    sDevicePtr masterDevice;           //! das Gerät ist MASTER / oder das Einzige
    bool alertIsRunning{true};         //! solange der Thread laufen soll
    QMutex mutex;
    QVector< waitForType > waitForList;           //! Auf was warte ich alles
    alertStatusType alertStatus{AL_INACTIVE};     //! in welchem Status ist der Automat
    alertStatusType alertOldStatus{AL_FINISHED};  //! in welchem Status war der Automat vor WAIT

    protected:
    void timerEvent( QTimerEvent *event ) override;

    public:
    explicit BoseSoundAlert( AppConfigPtr appconfig, const SingleAlertConfig &alConf, LoggerPtr lgr, QObject *parent = nullptr );
    ~BoseSoundAlert() override;    //! der Zerstörer
    void run() override;           //! hier findet sann alles statt
    QString getAlertName() const;  //! drt Name des alarms
    void cancelAlert();            //! breche den Alarm ab (von extern oder intern)
    inline static quint16 getAlertCount()
    {
      return alertCount;
    };  //! erfrage die Anzahl aktiver Alarme

    private:
    void prepareAlert();               //! Bereite Alarm vor AL_PREPARE1 VORBEREITUNG
    void prepareAlertCheckStandby();   //! Bereite Alarm vor AL_PREPARE2 STANDBY
    void prepareAlertGetVolume();      //! Bereite Alarm vor AL_PREPARE3 GET VOLUME
    void prepareAlertSetVolume();      //! Bereite Alarm vor AL_PREPARE4 SET VOLUME
    void prepareAlertSetTuner();       //! Bereite Alarm vor AL_PREPARE5 SET TUNER
    void prepareAlertWaitForSource();  //! Bereite Alarm vor AL_PREPARE6 warte auf TUNER
    waitState waitForSetVolume();      //! Warte ob die Lautstärke gesettz wurde
    void raiseVolume();                //! erhöhe die Lautstärke AL_RAISE
    void checkPlaying();               //! überwache den Ablauf beim abspielen AL_PLAY
    void reduceVolume();               //! verringere Lautstärke AL_REDUCE
    void postpareAlert();              //! Aufräumarbeiten /restaurieren des Zustandes und Ausschalten AL_POSTPARE
    void finishAlert();                //! Alarm beenden AL_FINISHED
    //
    bool buildGroup();          //! erzeuge eine playgroup
    bool askForStandby();       //! ist das Gerät im Standby? true => Ja
    bool askForVolume();        //! blockierend, wartet auf Lautstärke
    bool setVolume( int vol );  //! blockierend, setze Lautstärke
    bool setTunerChannel();     //! stelle den gewählten Sender (aus der config) ein
    signals:

    private slots:
    // Asynchrone Nachrichten der Geräte
    void slotOnPresetsUpdated( ResponseObjPtr response );
    void slotOnNowPlayingUpdated( ResponseObjPtr response );
    void slotOnPresetSelectionUpdated( ResponseObjPtr response );
    void slotOnVolumeUpdated( ResponseObjPtr response );
    void slotOnBassUpdated( ResponseObjPtr response );
    void slotOnZoneUpdated( ResponseObjPtr response );
    void slotOnInfoUpdated( ResponseObjPtr response );
    void slotOnNameUpdated( ResponseObjPtr response );
    void slotOnErrorUpdated( ResponseObjPtr response );
    void slotOnGroupUpdated( ResponseObjPtr response );
  };
}  // namespace bose_commserver
#endif  // BOSESOUNDALERT_HPP
