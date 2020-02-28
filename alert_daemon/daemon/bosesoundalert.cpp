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
    *lg << LDEBUG << "BoseSoundAlert::BoseSoundAlert: construct <" << alConfig.getName() << "> OK" << endl;
    mainTimerId = startTimer( 100 );
  }

  BoseSoundAlert::~BoseSoundAlert()
  {
    *lg << LDEBUG << "BoseSoundAlert::~BoseSoundAlert: Alert <" << alConfig.getName() << "> destroing..." << endl;
  }

  void BoseSoundAlert::run()
  {
    *lg << LINFO << "BoseSoundAlert::run: thread start...";
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
    *lg << LINFO << "BoseSoundAlert::run: thread end..." << endl;
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
      *lg << LDEBUG << QString( "BoseSoundAlert::timerEvent: timer event %2..." ).arg( durationCounter, 3, 10, QChar( '0' ) ) << endl;
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
    *lg << LINFO << "BoseSoundAlert::cancelAlert: force to cancel Alert!" << endl;
    //
    // Alarm beenden!
    //
    alertIsRunning = false;
  }
}  // namespace bose_commserver
