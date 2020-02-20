#include "daemontimer.hpp"

namespace bose_commserver
{
  DaemonTimer::DaemonTimer( AppConfigPtr dconfig, QObject *parent )
      : QObject( parent ), config( dconfig ), lg( dconfig->getLogger() ), alList( dconfig->getAlConfigs() ), timerCounter( 0 )
  {
    //
    // initialisiere und starte den Timer des Objektes
    //
    lg->debug( "DaemonTimer::DaemonTimer: set timer..." );
    ticker.setInterval( TIMER_INTERVAL );
    lg->debug( "DaemonTimer::DaemonTimer: connect signals..." );
    connect( &ticker, &QTimer::timeout, this, &DaemonTimer::onTimerTimeout );
    lg->debug( "DaemonTimer::DaemonTimer: connect signals...OK" );
    ticker.start();
    lg->debug( "DaemonTimer::DaemonTimer: timer started..." );
  }

  /**
   * @brief DaemonTimer::stopTimer
   */
  void DaemonTimer::stopTimer()
  {
    ticker.stop();
    lg->debug( "DaemonTimer::stopTimer: timer stopped..." );
  }

  /**
   * @brief DaemonTimer::onTimerTimeout
   */
  void DaemonTimer::onTimerTimeout()
  {
    timerCounter++;
    //
    // teste die Alarme
    // immer alle 10 Ticks (sollte dann 10 Sekunden sein)
    //
    if ( timerCounter % 10 == 0 )
    {
      lg->debug( "DaemonTimer::onTimerTimeout: check alerts..." );
      QDate today( QDate::currentDate() );
      QTime now( QTime::currentTime() );
      QDateTime toDayNow( QDateTime::currentDateTime() );
      //
      // alle durch
      //
      for ( auto &currAl : *alList )
      {
#ifdef DEBUG
        lg->debug( QString( "DaemonTimer::onTimerTimeout: check alert <%1>" ).arg( currAl.getName() ) );
#endif
        //
        // starte Alarm, wenn er an der reihe ist
        //
        testIfAlertNeedStart( today, now, currAl );
        //
        // abgelaufener Alarm?
        // länger als eine Stunde her?
        //
        if ( currAl.getRunSince().secsTo( toDayNow ) > ( 60 * 60 ) )
        {
          //
          // der ist länger als eine Sekunde aktiv!
          //
          lg->info( QString( "DaemonTimer::onTimerTimeout: emit signale for stop alert <%1>" ).arg( currAl.getName() ) );
          emit sigStopAlert( currAl );
        }
      }  // ende schleife über alle Alarme
    }
    //
    // den Zähler zurücksetzten
    //
    if ( timerCounter > 98 )
      timerCounter = 0;
    // ende
  }

  void stopAlert( SingleAlertConfig &alert );
  /**
   * @brief DaemonTimer::testIfAlertNeedStart
   * @param today
   * @param now
   * @param currAl
   */
  void DaemonTimer::testIfAlertNeedStart( QDate &today, QTime &now, SingleAlertConfig &currAl )
  {
    //
    // ist der Alarm mit Datum, und wenn ja, ist er heute?
    //
    if ( currAl.getAlDate().isValid() )
    {
      //
      // Datum ist gültig, ist es auch heute?
      //
      if ( currAl.getAlDate() == today )
      {
        lg->debug( QString( "DaemonTimer::onTimerTimeout: alert <%1> is today! (at %2)" )
                       .arg( currAl.getName() )
                       .arg( currAl.getAlDate().toString( command::dateFormatStr ) ) );
        //
        // wenn der Alarm nicht läuft (NICHT isValid)
        // und der Zeitpunkt zwischen + 45 und - 15 Sekunden ist
        //
        startAlertIfTime( currAl, now );
      }
      //
      // wenn der Alarm nicht für heute und jetzt war...
      // ist schon schluss, nächster Alarm
      //
    }
    else
    {
      //
      // kein Datum, also täglich oder an Wochentagen
      //
      if ( !currAl.getAlDays().isEmpty() )
      {
        //
        // da sind evtl Wochentage, also nicht täglich
        // ist der heutige Wochentag in der Liste?
        //
        if ( currAl.getAlDays().contains( static_cast< qint8 >( today.dayOfWeek() ) ) )
        {
          lg->debug( QString( "DaemonTimer::onTimerTimeout: alert <%1> weekday is today! Now check time to start..." )
                         .arg( currAl.getName() ) );
          //
          // wenn der Alarm nicht läuft (NICHT isValid)
          // und der Zeitpunkt zwischen + 45 und - 15 Sekunden ist
          //
          startAlertIfTime( currAl, now );
        }
      }
      else
      {
        lg->debug( QString( "DaemonTimer::onTimerTimeout: alert <%1> -> Now check time to start..." ).arg( currAl.getName() ) );
        //
        // keine Wochentage eingetragen, kein Datum, warscheinlich taegliche Ausführung
        // wenn der Alarm nicht läuft (NICHT isValid)
        // und der Zeitpunkt zwischen + 45 und - 15 Sekunden ist
        //
        startAlertIfTime( currAl, now );
      }
    }
  }

  /**
   * @brief DaemonTimer::startAlertIfTime
   * @param currAl
   * @param now
   */
  void DaemonTimer::startAlertIfTime( SingleAlertConfig &currAl, QTime &now )
  {
    //
    // wenn der Alarm nicht läuft (NICHT isValid)
    // und der Zeitpunkt zwischen + 45 und - 15 Sekunden ist
    //
    int distanceToAlert = currAl.getAlTime().secsTo( now );
    int diffToZero = distanceToAlert - 45;
    int maxDistance = 60;
#ifdef DEBUG
    int alToNow = currAl.getAlTime().secsTo( now );
    int nowToAl = now.secsTo( currAl.getAlTime() );
    lg->debug( QString( "DaemonTimer::startAlertIfTime: alert to now: <%1> now to alert: <%2> distance <%3>" )
                   .arg( alToNow )
                   .arg( nowToAl )
                   .arg( diffToZero ) );
#endif
    //
    if ( !currAl.getRunSince().isValid() && ( diffToZero >= 0 && diffToZero < maxDistance ) )
    {
      //
      // starte den Alarm!
      //
      lg->debug( QString( "DaemonTimer::startAlertIfTime: alert <%1> start now!" ).arg( currAl.getName() ) );
      emit sigStartAlert( currAl );
    }
  }

}  // namespace bose_commserver
