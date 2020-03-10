#include "bosesoundalert.hpp"

using namespace bose_soundtoch_lib;

namespace bose_commserver
{
  const QRegExp BoseSoundAlert::rePreset( "^PRESET_[123456]$" );
  const QRegExp BoseSoundAlert::rePresetNum( "^PRESET_([123456])$" );
  const QRegExp BoseSoundAlert::reInet( "INTERNET_RADIO|TUNEIN" );
  const QRegExp BoseSoundAlert::reAmazon( "^AMAZON$" );
  const QRegExp BoseSoundAlert::reStandby( "^STANDBY$" );

  /**
   * @brief BoseSoundAlert::BoseSoundAlert
   * @param appconfig
   * @param alConf
   * @param lgr
   * @param parent
   */
  BoseSoundAlert::BoseSoundAlert( AppConfigPtr appconfig, const SingleAlertConfig &alConf, LoggerPtr lgr, QObject *parent )
      : QThread( parent )
      , alConfig( alConf )
      , config( appconfig )
      , lg( lgr )
      , duration( alConf.getAlDuration() )
      , durationCounter( static_cast< qint32 >( alConf.getAlDuration() ) * factorTo1Sec )
      , alertVolume( alConf.getAlVolume() )
  {
    *lg << LDEBUG << "BoseSoundAlert::BoseSoundAlert: construct <" << alConfig.getName() << "> OK" << endl;
    mainTimerId = startTimer( timerValue );
    alertStatus = AL_PREPARE;
  }

  /**
   * @brief BoseSoundAlert::~BoseSoundAlert
   */
  BoseSoundAlert::~BoseSoundAlert()
  {
    *lg << LDEBUG << "BoseSoundAlert::~BoseSoundAlert: Alert <" << alConfig.getName() << "> destroing..." << endl;
  }

  /**
   * @brief BoseSoundAlert::run Hauptschleife des Threads
   */
  void BoseSoundAlert::run()
  {
    *lg << LINFO << "BoseSoundAlert::run: thread start...";
    //
    // warte bis der Thread beendet werden soll
    //
    while ( alertIsRunning )
      msleep( 80 );
    *lg << LINFO << "BoseSoundAlert::run: thread end..." << endl;
  }

  /**
   * @brief BoseSoundAlert::timerEvent
   * @param event
   */
  void BoseSoundAlert::timerEvent( QTimerEvent * )
  {
    //
    // verzweige in die richtige Routine für den jeweiligen Status des Automaten
    //
    switch ( alertStatus )
    {
      case AL_INACTIVE:
        alertStatus = AL_PREPARE;
        break;
      case AL_PREPARE:
        prepareAlert();
        break;
      case AL_RAISE:
        durationCounter--;
        raiseVolume();
        break;
      case AL_PLAY:
        durationCounter--;
        checkPlaying();
        break;
      case AL_REDUCE:
        reduceVolume();
        break;
      case AL_POSTPARE:
        postpareAlert();
        break;
      case AL_FINISHED:
        alertIsRunning = false;
        break;
      case AL_WAITFOR:
        // TODO: ist das erwartete Ereignis eingetreten oder timeout
        break;
    }
  }

  /**
   * @brief BoseSoundAlert::prepareAlert
   */
  void BoseSoundAlert::prepareAlert()
  {
    static bool isWorking = false;
    if ( isWorking )
      return;
    isWorking = true;
    //
    // Die geforderten Geräte mit den vorhandenen vergleichen
    // TODO: läuft das MASTER Gerät schon, ABBRECHEN
    //
    QStringList devNames = alConfig.getAlDevices();
    const SoundTouchDevicesList &availDevices = config->getAvailDevices();
    //
    for ( const QString &reqName : devNames )
    {
      for ( auto avlDevice : availDevices )
      {
        if ( avlDevice.getName().compare( reqName ) == 0 )
        {
          //
          // das geforderte Gerät ist tatsächlich da!
          // ein Objekt für den Alarm erstellen und in die Liste einbringen
          //
          sDevicePtr sDevice = sDevicePtr( new BSoundTouchDevice(
              avlDevice.getHostName(), avlDevice.getWsPort(), avlDevice.getPort(), this,
              config->getThreshold() == LgThreshold::LG_DEBUG ? QtMsgType::QtDebugMsg : QtMsgType::QtCriticalMsg ) );
          sDevice->setId( avlDevice.getId() );
          sDevice->setIpString( avlDevice.getIp().toString() );
          //
          // Das Gerät erst mal mit allen Slots verbinden
          //
          connect( sDevice.get(), &BSoundTouchDevice::sigOnPresetsUpdated, this, &BoseSoundAlert::slotOnPresetsUpdated );
          connect( sDevice.get(), &BSoundTouchDevice::sigOnNowPlayingUpdated, this, &BoseSoundAlert::slotOnNowPlayingUpdated );
          connect( sDevice.get(), &BSoundTouchDevice::sigOnPresetSelectionUpdated, this,
                   &BoseSoundAlert::slotOnPresetSelectionUpdated );
          connect( sDevice.get(), &BSoundTouchDevice::sigOnVolumeUpdated, this, &BoseSoundAlert::slotOnVolumeUpdated );
          connect( sDevice.get(), &BSoundTouchDevice::sigOnBassUpdated, this, &BoseSoundAlert::slotOnBassUpdated );
          connect( sDevice.get(), &BSoundTouchDevice::sigOnZoneUpdated, this, &BoseSoundAlert::slotOnZoneUpdated );
          connect( sDevice.get(), &BSoundTouchDevice::sigOnInfoUpdated, this, &BoseSoundAlert::slotOnInfoUpdated );
          connect( sDevice.get(), &BSoundTouchDevice::sigOnNameUpdated, this, &BoseSoundAlert::slotOnNameUpdated );
          connect( sDevice.get(), &BSoundTouchDevice::sigOnErrorUpdated, this, &BoseSoundAlert::slotOnErrorUpdated );
          connect( sDevice.get(), &BSoundTouchDevice::sigOnGroupUpdated, this, &BoseSoundAlert::slotOnGroupUpdated );
          // in die Liste
          devices.append( sDevice );
        }
      }  // for availDevices
    }    // for devnames
    //
    // wie viele Geräte gefunden?
    //
    int countDevices = devices.count();
    *lg << LDEBUG << "BoseSoundAlert::prepareAlert: found <" << countDevices << "> device(s) for altert" << endl;
    if ( countDevices == 0 )
    {
      // kein Gerät!
      *lg << LCRIT << "found non device for alert! abort!" << endl;
      alertStatus = AL_FINISHED;
      isWorking = false;
      return;
    }
    if ( countDevices > 1 )
    {
      if ( !buildGroup() )
      {
        alertStatus = AL_FINISHED;
        *lg << LWARN << "can't create playgroup for alert! try single on master device..." << endl;
        // single versuchen?
        // master device wir ja trotzdem gesetzt
        // return;
      }
    }
    else
    {
      //
      // genau ein Gerät
      //
      masterDevice = devices.first();
    }
    //
    // verbinde das Master Gerät
    //
    masterDevice->connectWs();
    //
    // teste ob das Gerät im Standby ist
    //
    if ( !askForStandby() )
    {
      //
      // GEHT NICHT; ABBRECHEN?!
      // TODO: was sinnvolles machen
      //
      *lg << LINFO << "master device is not in state STANDBY => abort alert" << endl;
      alertStatus = AL_FINISHED;
      isWorking = false;
      return;
    }
    //
    // Lautstärke erfragen (asynchron, blockierend bis timeout oder Ergebnis)
    //
    if ( !askForVolume() )
    {
      //
      // GEHT NICHT; ABBRECHEN?!
      // TODO: was sinnvolles machen
      //
      alertStatus = AL_FINISHED;
      isWorking = false;
      return;
    }
    //
    // Ausgangslautstärke merken
    //
    bevorVolume = currentVolume;
    if ( alConfig.getAlRaiseVolume() )
    {
      if ( !setVolume( 0 ) )
      {
        //
        // GEHT NICHT; ABBRECHEN?!
        // TODO: was sinnvolles machen
        //
        alertStatus = AL_FINISHED;
        isWorking = false;
        return;
      }
      if ( currentVolume == 0 )
      {
        *lg << LDEBUG << "BoseSoundAlert::prepareAlert: volume set to raise from 0" << endl;
      }
      else
      {
        *lg << LWARN << "volume while prepare raising set 0, recived: <" << currentVolume << ">" << endl;
      }
    }
    //
    // Sender einstellen
    //
    if ( !setTunerChannel() )
    {
      //
      // GEHT NICHT, versuche PRESET_1
      //
      alConfig.setAlSource( "PRESET_1" );
      if ( !setTunerChannel() )
      {
        //
        // GEHT NICHT; ABBRECHEN?!
        // TODO: was sinnvolles machen
        //
        alertStatus = AL_POSTPARE;
        isWorking = false;
        return;
      }
    }
    //
    // ab hier sollte alles laufen,
    // jetzt entscheiden was weiter passiert
    //
    if ( alConfig.getAlRaiseVolume() )
      alertStatus = AL_RAISE;
    else
      alertStatus = AL_PLAY;
    isWorking = false;
  }

  /**
   * @brief BoseSoundAlert::raiseVolume
   */
  void BoseSoundAlert::raiseVolume()
  {
    static bool isWorking = false;
    if ( isWorking )
      return;
    isWorking = true;
    //
    // ist das Ziel erreicht?
    //
    if ( alertVolume > 0 && alertVolume == currentVolume )
    {
      //
      // neuer Status ist spielen
      //
      alertStatus = AL_PLAY;
      isWorking = false;
      return;
    }
    //
    // da muss ich also die Lautstärke erhöhen
    //
    if ( !setVolume( currentVolume + alertRaiseStep ) )
    {
      //
      // GEHT NICHT; ABBRECHEN?!
      // TODO: was sinnvolles machen
      //
      alertStatus = AL_FINISHED;
      isWorking = false;
      return;
    }
    isWorking = false;
  }

  void BoseSoundAlert::checkPlaying()
  {
  }

  void BoseSoundAlert::reduceVolume()
  {
    static bool isWorking = false;
    if ( isWorking )
      return;
    isWorking = true;
    //
    // ist das Ziel erreicht?
    //
    if ( currentVolume == 0 )
    {
      //
      // neuer Status ist aufräumen
      //
      alertStatus = AL_POSTPARE;
      isWorking = false;
      return;
    }
    //
    // da muss ich also die Lautstärke verringern
    //
    int newVolume = currentVolume - alertRaiseStep;
    if ( newVolume < 0 )
      newVolume = 0;
    if ( !setVolume( newVolume ) )
    {
      //
      // GEHT NICHT; ABBRECHEN?!
      // TODO: was sinnvolles machen
      //
      alertStatus = AL_POSTPARE;
      isWorking = false;
      return;
    }
    isWorking = false;
  }

  void BoseSoundAlert::postpareAlert()
  {
  }

  void BoseSoundAlert::finishAlert()
  {
  }

  bool BoseSoundAlert::buildGroup()
  {
    *lg << LDEBUG << "BoseSoundAlert::buildGroup: found <" << devices.count() << ">, build an group..." << endl;
    //
    // mehr als ein Gerät: ich muss eine Gruppe bilden
    //
    masterDevice = devices.first();
    //
    // erzeuge die Gruppe
    //
    SoundTouchMemberList memberList;
    for ( auto cDev : devices )
    {
      if ( cDev == masterDevice )
        continue;
      //! Paare Soundtouch <"IPADDR", "MACADDR/DEVICEID">
      // using SoundTouchMemberObject = QPair< QString, QString >;
      memberList.append( SoundTouchMemberObject( cDev->getIpString(), cDev->getId() ) );
    }
    masterDevice->setZone( masterDevice->getId(), memberList );
    // TODO: warte auf Bestätigung mit Timeout
    return ( true );
  }

  void BoseSoundAlert::slotOnPresetsUpdated( ResponseObjPtr response )
  {
    WsPresetUpdateObject *pres = static_cast< WsPresetUpdateObject * >( response.get() );
    *lg << LINFO << "on device " << pres->getDeviceId() << " pressed " << pres->getDevicePresets().first().id << " alert aborting"
        << endl;
    // alertStatus = AL_FINISHED;
  }

  void BoseSoundAlert::slotOnNowPlayingUpdated( ResponseObjPtr response )
  {
    WsNowPlayingUpdate *npl = static_cast< WsNowPlayingUpdate * >( response.get() );
    //
    // wartet jemand auf Senderwechsel
    //
    int wasWaiting = waitForList.indexOf( WT_SOURCE );
    if ( wasWaiting != -1 )
    {
      if ( currentSource.compare( npl->getSource() ) != 0 )
      {
        //
        // es fand ein wechsel statt
        //
        currentSource = npl->getSource();
        waitForList.remove( wasWaiting );
        waitingForAnswer.wakeAll();
      }
    }
    //
    // wartet jemand auf STATUSWECHSEL
    //
    wasWaiting = waitForList.indexOf( WT_PLAYSTATE );
    if ( wasWaiting != -1 )
    {
      if ( currentPlaystate.compare( npl->getPlayStatus() ) != 0 )
      {
        //
        // es fand ein wechsel statt
        //
        currentPlaystate = npl->getPlayStatus();
        waitForList.remove( wasWaiting );
        waitingForAnswer.wakeAll();
      }
    }
    //
    // wartet jemand auf STANDBY
    //
    wasWaiting = waitForList.indexOf( WT_STANDBY );
    if ( wasWaiting != -1 )
    {
      if ( reStandby.exactMatch( npl->getPlayStatus() ) )
      {
        //
        // STANDBY eingetroffen
        //
        currentPlaystate = npl->getPlayStatus();
        waitForList.remove( wasWaiting );
        waitingForAnswer.wakeAll();
      }
    }
    //
    // speichere den aktuellen Status, soweit noch nicht passiert
    //
    currentPlaystate = npl->getPlayStatus();
    currentSource = npl->getSource();
    //
    *lg << LDEBUG << "BoseSoundAlert::slotOnNowPlayingUpdated: playing status on device " << npl->getDeviceId()
        << " now: " << npl->getPlayStatus() << endl;
  }

  void BoseSoundAlert::slotOnPresetSelectionUpdated( ResponseObjPtr response )
  {
    WsNowSelectionUpdated *nsu = static_cast< WsNowSelectionUpdated * >( response.get() );
    *lg << LDEBUG << "BoseSoundAlert::slotOnPresetSelectionUpdated: preset on device : " << nsu->getDeviceId()
        << " switched to: " << nsu->getDevicePresets().id << endl;
    // TODO: info benutzen
  }

  void BoseSoundAlert::slotOnVolumeUpdated( ResponseObjPtr response )
  {
    WsVolumeUpdated *vol = static_cast< WsVolumeUpdated * >( response.get() );
    //
    // aktuelle Lautstärke speuchern
    //
    currentVolume = vol->getActualVolume();
    //
    // wenn jemand darauf wartet, von der Warteliste streichen und
    // alle benachrichtigen
    //
    int wasWaiting = waitForList.indexOf( WT_VOLUME );
    if ( wasWaiting != -1 )
    {
      waitForList.remove( wasWaiting );
      waitingForAnswer.wakeAll();
    }
    *lg << LDEBUG << "BoseSoundAlert::slotOnVolumeUpdated: on device : " << vol->getDeviceId()
        << " voulume updated to: " << vol->getActualVolume() << endl;
    // TODO: zur überwachung nutzen
  }

  void BoseSoundAlert::slotOnBassUpdated( ResponseObjPtr response )
  {
    WsBassUpdated *bu = static_cast< WsBassUpdated * >( response.get() );
    *lg << LDEBUG << "BoseSoundAlert::slotOnBassUpdated: bass updatet on device: " << bu->getDeviceId()
        << " to value: " << bu->getUpdatet() << endl;
    // Nur INFO
  }

  void BoseSoundAlert::slotOnZoneUpdated( ResponseObjPtr response )
  {
    WsZoneUpdated *zu = static_cast< WsZoneUpdated * >( response.get() );
    *lg << LINFO << "zone update on device: " << zu->getDeviceId() << endl;
    // TODO: zur überwachung der Kommandos
  }

  void BoseSoundAlert::slotOnInfoUpdated( ResponseObjPtr response )
  {
    WsInfoUpdated *iu = static_cast< WsInfoUpdated * >( response.get() );
    *lg << LDEBUG << "BoseSoundAlert::slotOnInfoUpdated: on device: " << iu->getDeviceId() << " info update..." << endl;
    // Nur info
  }

  void BoseSoundAlert::slotOnNameUpdated( ResponseObjPtr response )
  {
    WsNameUpdated *nu = static_cast< WsNameUpdated * >( response.get() );
    *lg << LDEBUG << "BoseSoundAlert::slotOnNameUpdated: device changed name to: " << nu->getDeviceId() << endl;
  }

  void BoseSoundAlert::slotOnErrorUpdated( ResponseObjPtr response )
  {
    WsErrorUpdated *eu = static_cast< WsErrorUpdated * >( response.get() );
    *lg << LCRIT << "error update on device: " << eu->getError().text << "on device" << eu->getDeviceId();
    // TODO: was machen
  }

  void BoseSoundAlert::slotOnGroupUpdated( ResponseObjPtr response )
  {
    WsGroupUpdated *gu = static_cast< WsGroupUpdated * >( response.get() );
    *lg << LDEBUG << "BoseSoundAlert::slotOnGroupUpdated: group update on device: " << gu->getDeviceId();
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

  /**
   * @brief BoseSoundAlert::askForVolume blockierend mit timeout
   * @return
   */
  bool BoseSoundAlert::askForVolume()
  {
    *lg << LDEBUG << "BoseSoundAlert::askForVolume: ..." << endl;
    answerMutex.lock();
    masterDevice->getVolume();
    //
    // in die Liste das warten auf Lautstärke eintragen, wenn nötig
    //
    if ( waitForList.indexOf( WT_VOLUME ) == -1 )
      waitForList.append( WT_VOLUME );
    //
    // und nun warten...
    //
    int vloops = 4;
    while ( waitForList.indexOf( WT_VOLUME ) != -1 && vloops > 0 )
    {
      waitingForAnswer.wait( &answerMutex, asyncTimeout );
      *lg << LWARN << "timeout while waiting for request for volume..." << endl;
      vloops--;
    }
    answerMutex.unlock();
    if ( vloops <= 0 )
    {
      *lg << LCRIT << "can't get answer for volume on master device. abort!" << endl;
      return false;
    }
    *lg << LDEBUG << "BoseSoundAlert::askForVolume: ...OK" << endl;
    return true;
  }

  /**
   * @brief BoseSoundAlert::askForStandby
   * @return
   */
  bool BoseSoundAlert::askForStandby()
  {
    *lg << LDEBUG << "BoseSoundAlert::askForStandby: ..." << endl;
    answerMutex.lock();
    masterDevice->getNowPlaying();
    //
    // in die Liste das warten auf standby eintragen
    //
    if ( waitForList.indexOf( WT_STANDBY ) == -1 )
      waitForList.append( WT_STANDBY );
    //
    // und nun warten...
    //
    int vloops = 2;
    while ( waitForList.indexOf( WT_STANDBY ) != -1 && vloops > 0 )
    {
      waitingForAnswer.wait( &answerMutex, asyncTimeout );
      *lg << LWARN << "timeout while waiting for request for standby..." << endl;
      vloops--;
    }
    answerMutex.unlock();
    if ( vloops <= 0 )
    {
      *lg << LCRIT << "can't get find standby on master device. abort!" << endl;
      return false;
    }
    *lg << LDEBUG << "BoseSoundAlert::askForStandby: ...OK" << endl;
    return true;
  }

  bool BoseSoundAlert::setVolume( int vol )
  {
    *lg << LDEBUG << "BoseSoundAlert::setVolume: set to: " << vol << "..." << endl;
    answerMutex.lock();
    masterDevice->setVolume( vol );
    //
    // in die Liste das warten auf Lautstärke eintragen, wenn nötig
    //
    if ( waitForList.indexOf( WT_VOLUME ) == -1 )
      waitForList.append( WT_VOLUME );
    //
    // und nun warten...
    //
    int vloops = 4;
    while ( waitForList.indexOf( WT_VOLUME ) != -1 && vloops > 0 )
    {
      waitingForAnswer.wait( &answerMutex, asyncTimeout );
      *lg << LWARN << "timeout while waiting for request for set volume..." << endl;
      vloops--;
    }
    answerMutex.unlock();
    if ( vloops <= 0 )
    {
      *lg << LCRIT << "can't get answer for set volume on master device. abort!" << endl;
      return false;
    }
    *lg << LDEBUG << "BoseSoundAlert::setVolume: ...OK" << endl;
    return true;
  }

  bool BoseSoundAlert::setTunerChannel()
  {
    QString source = alConfig.getAlSource();
    *lg << LDEBUG << "BoseSoundAlert::setTunerChannel: set to: " << source << "..." << endl;
    answerMutex.lock();
    //
    // welche Art Source
    //
    // "^PRESET_[123456]$"
    // "INTERNET_RADIO|TUNEIN"
    // "^AMAZON$"
    // "^STANDBY$"

    if ( rePreset.exactMatch( source ) )
    {
      *lg << LINFO << "playing PRESET <" << source << ">..." << endl;
      //
      // es soll ein PRESET gespielt werden, finde die Nummer heraus
      //
      int captured = rePresetNum.indexIn( source );
      if ( captured > 1 )
      {
        //
        // da war was...
        // die Nummer des Preset
        //
        int preset = rePresetNum.capturedTexts()[ 1 ].toInt();
        *lg << LDEBUG << "BoseSoundAlert::setTunerChannel: Number of PRESET: <" << preset << ">..." << endl;
        //
        // Korrekte Keybezeichnung finden
        //
        BSoundTouchDevice::bose_key whichkey;
        switch ( preset )
        {
          case 1:
            whichkey = BSoundTouchDevice::bose_key::KEY_PRESET_1;
            break;
          case 2:
            whichkey = BSoundTouchDevice::bose_key::KEY_PRESET_2;
            break;
          case 3:
            whichkey = BSoundTouchDevice::bose_key::KEY_PRESET_3;
            break;
          case 4:
            whichkey = BSoundTouchDevice::bose_key::KEY_PRESET_4;
            break;
          case 5:
            whichkey = BSoundTouchDevice::bose_key::KEY_PRESET_5;
            break;
          case 6:
            whichkey = BSoundTouchDevice::bose_key::KEY_PRESET_6;
            break;
          default:
            whichkey = BSoundTouchDevice::bose_key::KEY_PRESET_1;
        }
        *lg << LDEBUG << "BoseSoundAlert::setTunerChannel: send keypress to master device..." << endl;
        masterDevice->setKey( whichkey, BSoundTouchDevice::bose_keystate::KEY_RELEASED );
      }
      else
      {
        *lg << LWARN << "can't find which preset you want. try default PRESET_1...";
        masterDevice->setKey( BSoundTouchDevice::bose_key::KEY_PRESET_1, BSoundTouchDevice::bose_keystate::KEY_RELEASED );
      }
    }
    else
    {
      //
      // Alle anderen Typen TODO: implementieren
      //
      *lg << LWARN << "can't play your source <" << source << ", try default PRESET_1...";
      masterDevice->setKey( BSoundTouchDevice::bose_key::KEY_PRESET_1, BSoundTouchDevice::bose_keystate::KEY_RELEASED );
    }
    //
    // und nun warten...
    // .. auf senderwechsel
    //
    if ( currentSource.compare( alConfig.getAlSource() ) != 0 )
    {
      //
      // in die Liste das warten auf SOURCE eintragen, wenn nötig
      //
      if ( waitForList.indexOf( WT_SOURCE ) == -1 )
        waitForList.append( WT_SOURCE );
      //
      // warten
      //
      int vloops = 6;
      while ( waitForList.indexOf( WT_SOURCE ) != -1 && vloops > 0 )
      {
        waitingForAnswer.wait( &answerMutex, asyncTimeout );
        *lg << LWARN << "timeout while waiting for request for set source..." << endl;
        vloops--;
      }
      answerMutex.unlock();
      if ( vloops <= 0 )
      {
        *lg << LCRIT << "can't get answer for set source on master device. abort!" << endl;
        return false;
      }
    }
    //
    // warte auf playstsatus "PLAY_STATE" BUFFERING_STATE sollte endlich sein...
    //
    if ( currentPlaystate.compare( "PLAY_STATE" ) != 0 )
    {
      //
      // in die Liste das warten auf PLAY eintragen, wenn nötig
      //
      if ( waitForList.indexOf( WT_PLAYSTATE ) == -1 )
        waitForList.append( WT_PLAYSTATE );
      //
      // warten
      //
      int vloops = 6;
      while ( waitForList.indexOf( WT_PLAYSTATE ) != -1 && vloops > 0 )
      {
        waitingForAnswer.wait( &answerMutex, asyncTimeout );
        *lg << LWARN << "timeout while waiting for playstate for set source..." << endl;
        vloops--;
      }
      answerMutex.unlock();
      if ( vloops <= 0 )
      {
        *lg << LCRIT << "can't get answer for playstate for set source. abort!" << endl;
        return false;
      }
    }

    *lg << LDEBUG << "BoseSoundAlert::setTunerChannel: ...OK" << endl;
    return true;
  }

}  // namespace bose_commserver
