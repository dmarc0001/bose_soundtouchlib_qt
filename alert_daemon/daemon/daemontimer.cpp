#include "daemontimer.hpp"

namespace bose_commserver
{
  DaemonTimer::DaemonTimer( AppConfigPtr dconfig, QObject *parent )
      : QObject( parent ), config( dconfig ), lg( dconfig->getLogger() ), alList( dconfig->getAlConfigs() )
  {
    //
    // initialisiere und starte den Timer des Objektes
    //
    *lg << LDEBUG << "DaemonTimer::DaemonTimer: set timer..." << endl;
    ticker.setInterval( TIMER_INTERVAL );
    *lg << LDEBUG << "DaemonTimer::DaemonTimer: connect signals..." << endl;
    connect( &ticker, &QTimer::timeout, this, &DaemonTimer::onTimerTimeout );
    *lg << LDEBUG << "DaemonTimer::DaemonTimer: connect signals...OK" << endl;
    ticker.start();
    *lg << LDEBUG << "DaemonTimer::DaemonTimer: timer started..." << endl;
  }

  /**
   * @brief DaemonTimer::stopTimer
   */
  void DaemonTimer::stopTimer()
  {
    //
    // da ich den Timer aus einem anderen Thread nicht einfach stoppen kann
    // dan kann ich wenigstens die ausführung der Routine stoppen
    //
    ignoreTimer = true;
    *lg << LDEBUG << "DaemonTimer::stopTimer: timer stopped..." << endl;
  }

  /**
   * @brief DaemonTimer::onTimerTimeout
   */
  void DaemonTimer::onTimerTimeout()
  {
    //
    // wen der Timer "aus" sein soll
    //
    if ( ignoreTimer )
      return;
    timerCounter++;
    //
    // teste die Alarme
    // immer alle x Ticks (sollte dann x Sekunden sein)
    //
    if ( timerCounter % 8 == 0 )
    {
      *lg << LDEBUG << "DaemonTimer::onTimerTimeout: check alerts..." << endl;
      QDate today( QDate::currentDate() );
      QTime now( QTime::currentTime() );
      QDateTime toDayNow( QDateTime::currentDateTime() );
      //
      // alle durch
      //
      for ( auto &currAl : *alList )
      {
#ifdef DEBUG
        *lg << LDEBUG << "DaemonTimer::onTimerTimeout: check alert <" << currAl.getName() << ">" << endl;
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
          *lg << LINFO << "DaemonTimer::onTimerTimeout: emit signale for stop alert <" << currAl.getName() << ">" << endl;
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
#ifdef DEBUG
    //
    // im debug mode auch den typ DEBUG quasi sofort starten
    // wenn der nicht schon läuft
    //
    if ( ( currAl.getAlType().compare( "DEBUG", Qt::CaseInsensitive ) == 0 ) && !currAl.getRunSince().isValid() )
    {
      currAl.setAlTime( QTime::currentTime().addSecs( 50 ) );
      currAl.setAlDate( QDate() );
      QList< qint8 > emptyList;
      currAl.setAlDays( emptyList );
    }
#endif
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
        *lg << LDEBUG
            << QString( "DaemonTimer::onTimerTimeout: alert <%1> is today! (at %2)" )
                   .arg( currAl.getName() )
                   .arg( currAl.getAlDate().toString( command::dateFormatStr ) )
            << endl;
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
          *lg << LDEBUG << "DaemonTimer::onTimerTimeout: alert <" << currAl.getName()
              << "> weekday is today! Now check time to start..." << endl;
          //
          // wenn der Alarm nicht läuft (NICHT isValid)
          // und der Zeitpunkt zwischen + 45 und - 15 Sekunden ist
          //
          startAlertIfTime( currAl, now );
        }
      }
      else
      {
        *lg << LDEBUG << "DaemonTimer::onTimerTimeout: alert <" << currAl.getName() << "> -> Now check time to start..." << endl;
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
    // wenn der Alarm nicht läuft (RunSinse ist NICHT isValid)
    // und der Zeitpunkt zwischen +/- 30 Sekunden ist
    //
    int distanceToAlert = now.secsTo( currAl.getAlTime() );
    int diffToZero = distanceToAlert - 30;
#ifdef DEBUG
    *lg << LDEBUG << "DaemonTimer::startAlertIfTime: now: " << now.toString( "HH:mm:ss" ) << ",  now to alert: <" << distanceToAlert
        << "s> distance <" << diffToZero << "s>" << endl;
#endif
    //
    // if ( !currAl.getRunSince().isValid() && ( distanceToAlert < 60 && distanceToAlert > 0 ) )
    if ( !currAl.getRunSince().isValid() && ( diffToZero < 60 && diffToZero > 0 ) )
    {
      //
      // starte den Alarm! (wenn er nicht schon läuft)
      //
      *lg << LDEBUG << "DaemonTimer::startAlertIfTime: alert <" << currAl.getName() << "> start now!" << endl;
      emit sigStartAlert( currAl );
    }
  }

}  // namespace bose_commserver
