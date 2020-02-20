#include "bosesoundalert.hpp"

namespace bose_commserver
{
  BoseSoundAlert::BoseSoundAlert( const SingleAlertConfig &alConf, LoggerPtr lgr, QObject *parent )
      : QThread( parent )
      , alConfig( alConf )
      , lg( lgr )
      , duration( alConf.getAlDuration() )
      , durationCounter( alConf.getAlDuration() )
      , alertIsRunning( true )
      , finishAlert( false )
  {
    lg->debug( QString( "BoseSoundAlert::BoseSoundAlert: construct <%1> OK" ).arg( alConfig.getName() ) );
    mainTimerId = startTimer( 1000 );
  }

  BoseSoundAlert::~BoseSoundAlert()
  {
    lg->debug( QString( "BoseSoundAlert::~BoseSoundAlert: Alert <%1> destroing..." ).arg( alConfig.getName() ) );
    // gib bescheid, damit der thread aus der Liste entfernt wird
    emit sigAlertFinish( alConfig.getName() );
  }

  void BoseSoundAlert::run()
  {
    lg->info( "BoseSoundAlert::run: thread start..." );
    //
    // Der Anfang...
    // TODO: initialisieren
    // initialisiere einiges
    // radio finden,
    // ws Verbindung aufbauen
    // lautstärke sichern
    // gruppe bilden (option)
    // sender wählen
    // fading (option)
    //
    // (durationCounter - raise time) wenn raise vol aktiv ist
    // TODO: wait
    while ( alertIsRunning )
      msleep( 250 );
    // mainTimer.stop();
    //
    // aufräumen
    // ausschalten
    // gruppe auflösen
    // lautstärke restaurieren
    // verbindung(en) lösen
    lg->info( "BoseSoundAlert::run: thread end..." );
  }

  /**
   * @brief BoseSoundAlert::timerEvent
   * @param event
   */
  void BoseSoundAlert::timerEvent( QTimerEvent *event )
  {
    lg->debug( QString( "BoseSoundAlert::timerEvent: timer event..." ).arg( event->timerId() ) );
  }

  /**
   * @brief BoseSoundAlert::getAlertName
   * @return
   */
  QString BoseSoundAlert::getAlertName() const
  {
    return alConfig.getName();
  }

  /**
   * @brief BoseSoundAlert::cancelAlert
   */
  void BoseSoundAlert::cancelAlert()
  {
    lg->info( "BoseSoundAlert::cancelAlert: force to cancel Alert!" );
    //
    // Alarm beenden!
    //
    alertIsRunning = false;
  }
}  // namespace bose_commserver
