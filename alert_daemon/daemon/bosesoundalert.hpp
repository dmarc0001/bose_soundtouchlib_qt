#ifndef BOSESOUNDALERT_HPP
#define BOSESOUNDALERT_HPP

#include <QList>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include <memory>

#include <bsoundtouchdevice.hpp>
#include "config/alertconfig.hpp"
#include "config/common_def.hpp"
#include "logging/logger.hpp"

namespace bose_commserver
{
  class BoseSoundAlert;

  using AlertTaskList = QList< BoseSoundAlert * >;
  using sDevicePtr = std::shared_ptr< bose_soundtoch_lib::BSoundTouchDevice >;

  class BoseSoundAlert : public QThread
  {
    Q_OBJECT

    private:
    SingleAlertConfig alConfig;   //! Kopie der Alarmkonfiguration
    LoggerPtr lg;                 //! der systemlogger
    qint16 duration;              //! der Startwert aus der config
    qint16 durationCounter;       //! wenn der auf 0 runter gezählt wird ist ENDE
    bool alertIsRunning;          //! solange true, alert läuft
    bool finishAlert;             //! beende den alarm, Aufräumen
    bool isVolumeRaising;         //! ist Lautstärke ansteigend und aufsteigend
    int mainTimerId{-1};          //! id des haupttimers
    qint8 alertVolume{0};         //! wenn vol raising dann hier für speed cachen
    qint8 currentVolume{0};       //! wenn vol raising dann hier für speed cachen
    QList< sDevicePtr > devices;  //! Geräte im Alarm, die Instanzen löschen sich selber

    protected:
    void timerEvent( QTimerEvent *event ) override;

    public:
    explicit BoseSoundAlert( const SingleAlertConfig &alConf, LoggerPtr lgr, QObject *parent = nullptr );
    ~BoseSoundAlert();             //! der Zerstörer
    void run() override;           //! hier findet sann alles statt
    QString getAlertName() const;  //! drt Name des alarms
    void cancelAlert();            //! breche den Alarm ab (von extern oder intern)

    private:
    void computeAlertNormal();  //! die normale Funktion, abwarten und überwachen
    signals:

    private slots:
  };
}  // namespace bose_commserver
#endif  // BOSESOUNDALERT_HPP
