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
    AppConfigPtr config;
    std::shared_ptr< Logger > lg;
    AlertListPtr alList;
    QTimer ticker;
    qint8 timerCounter;

    public:
    explicit DaemonTimer( AppConfigPtr dconfig, QObject *parent = nullptr );

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
