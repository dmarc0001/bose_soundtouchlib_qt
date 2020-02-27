#ifndef BOSESOUNDALERT_HPP
#define BOSESOUNDALERT_HPP

#include <QList>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include <memory>

#include "config/alertconfig.hpp"
#include "config/common_def.hpp"
#include "logging/logger.hpp"

namespace bose_commserver
{
  class BoseSoundAlert;

  using BoseSoundAlertPtr = std::shared_ptr< BoseSoundAlert >;
  using AlertTaskList = QList< BoseSoundAlertPtr >;

  class BoseSoundAlert : public QThread
  {
    Q_OBJECT
    private:
    SingleAlertConfig alConfig;  //! Kopie der Alarmkonfiguration
    LoggerPtr lg;                //! der systemlogger
    qint16 duration;             //! der Startwert aus der config
    qint16 durationCounter;      //! wenn der auf 0 runter gezählt wird ist ENDE
    bool alertIsRunning;         //! solange true, alert läuft
    bool finishAlert;            //! beende den alarm, Aufräumen
    int mainTimerId{-1};         //! id des haupttimers
    protected:
    void timerEvent( QTimerEvent *event ) override;

    public:
    explicit BoseSoundAlert( const SingleAlertConfig &alConf, LoggerPtr lgr, QObject *parent = nullptr );
    ~BoseSoundAlert();             //! der Zerstörer
    void run() override;           //! hier findet sann alles statt
    QString getAlertName() const;  //! drt Name des alarms
    void cancelAlert();            //! breche den Alarm ab (von extern oder intern)

    signals:
    void sigAlertFinish( const QString &alName );  //! senden wenn der alarm vorüber ist
  };
}  // namespace bose_commserver
#endif  // BOSESOUNDALERT_HPP
