#include "bosesoundalert.hpp"

using namespace bose_soundtoch_lib;

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
      , isVolumeRaising( alConf.getAlRaiseVolume() )
      , alertVolume( alConf.getAlVolume() )
  {
    lg->debug( QString( "BoseSoundAlert::BoseSoundAlert: construct <%1> OK" ).arg( alConfig.getName() ) );
    mainTimerId = startTimer( 100 );
  }

  BoseSoundAlert::~BoseSoundAlert()
  {
    lg->debug( QString( "BoseSoundAlert::~BoseSoundAlert: Alert <%1> destroing..." ).arg( alConfig.getName() ) );
  }

  void BoseSoundAlert::run()
  {
    lg->info( "BoseSoundAlert::run: thread start..." );
    /*
    QStringList devNames = alConfig.getAlDevices();
    for ( const QString &name : devNames )
    {
      sDevicePtr sDevice =
          sDevicePtr( new BSoundTouchDevice( host, wsPort, httpPort, this, threshold ) ) std::unique_ptr< BSoundTouchDevice > sDevice;
    }
    */
    //
    // wenn ansteigendes Volume gefordert war, einen wert für ticks per Lautstärkestufe
    // irgendwie zusammenbauen
    //
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
      msleep( 150 );
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
  void BoseSoundAlert::timerEvent( QTimerEvent * )
  {
    //
    if ( alertIsRunning )
    {
      if ( isVolumeRaising && alertVolume < currentVolume )
      {
        //
        // steigere um stepWith
        //
        // setDeviceVolume(currentVolume + stepwith)
      }
      /*
       * was schlaues zum ausblenden machen
       *
      else if ( isVolumeRaising && ( durationCounter - STEPS_TO_VOL ) < 0 )
      {
        //
        // verringere volume
        //
        // setDeviceVolume(currentVolume - stepWith)
      }
      */
      else
      {
        computeAlertNormal();
      }
    }
    if ( durationCounter <= 0 )
    {
      alertIsRunning = false;
    }
  }

  void BoseSoundAlert::computeAlertNormal()
  {
    static qint8 preCounter = 0;
    if ( ++preCounter % 10 == 0 )
    {
      lg->debug( QString( "BoseSoundAlert::timerEvent: timer event %2..." ).arg( durationCounter, 3, 10, QChar( '0' ) ) );
      durationCounter--;
      //
      // TODO: mach hier was
      //
    }
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
