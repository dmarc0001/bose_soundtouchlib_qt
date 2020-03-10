#ifndef DAEMONTIMER_HPP
#define DAEMONTIMER_HPP

#include <QObject>
#include <QTimer>

#include "config/alertconfig.hpp"
#include "config/common_def.hpp"

namespace bose_commserver
{
  class DaemonTimer : public QObject
  {
    Q_OBJECT
    private:
    bool ignoreTimer{false};  //! wenn der Timer gestoppt werden soll
    AppConfigPtr config;      //! Zeiger auf die Konfiguration
    LoggerPtr lg;             //! Zeiger auf den Logger
    AlertListPtr alList;      //! Zeiger auf die Liste mit Alarmen (aus config)
    QTimer ticker;            //! DAs Zeitgeber Objekt
    qint8 timerCounter{0};    //! Der Zähler für den Timer
    public:
    explicit DaemonTimer( AppConfigPtr dconfig, QObject *parent = nullptr );
    void stopTimer();

    private:
    void testIfAlertNeedStart( QDate &today, QTime &now, SingleAlertConfig &currAl );
    void startAlertIfTime( SingleAlertConfig &currAl, QTime &now );

    signals:
    void sigStartAlert( SingleAlertConfig &alert );
    void sigStopAlert( SingleAlertConfig &alert );

    private slots:
    void onTimerTimeout();
  };
}  // namespace bose_commserver
#endif  // DAEMONTIMER_HPP
