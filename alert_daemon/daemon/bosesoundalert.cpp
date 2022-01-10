#include "bosesoundalert.hpp"

using namespace bose_soundtoch_lib;

namespace bose_commserver
{
  const QRegExp BoseSoundAlert::rePreset( "^PRESET_[123456]$" );
  const QRegExp BoseSoundAlert::rePresetNum( "^PRESET_([123456])$" );
  const QRegExp BoseSoundAlert::reInet( "INTERNET_RADIO|TUNEIN" );
  const QRegExp BoseSoundAlert::reAmazon( "^AMAZON$" );
  const QRegExp BoseSoundAlert::reStandby( "^STANDBY$" );
  quint16 BoseSoundAlert::alertCount{0};

  /**
   * @brief BoseSoundAlert::BoseSoundAlert
   * @param appconfig
   * @param alConf
   * @param lgr
   * @param parent
   */
  BoseSoundAlert::BoseSoundAlert( AppConfigPtr appconfig, const SingleAlertConfig &alConf, LoggerPtr lgr, QObject *parent )
      : QObject( parent )
      , alConfig( alConf )
      , config( appconfig )
      , lg( lgr )
      , duration( static_cast< quint16 >( alConf.getAlDuration() ) )
      , durationCounter( static_cast< quint32 >( alConf.getAlDuration() ) * factorTo1Sec )
      , alertVolume( alConf.getAlVolume() )
  {
    ++BoseSoundAlert::alertCount;
    *lg << LDEBUG << "BoseSoundAlert::BoseSoundAlert: construct <" << alConfig.getName() << ">, No: <" << BoseSoundAlert::alertCount
        << "> OK" << Qt::endl;
    connect( &alTimer, &QTimer::timeout, this, &BoseSoundAlert::onTimeout );
    alertStatus = AL_PREPARE2;
    // Geräte vorbereiten
    prepareAlert();
    alTimer.start( timerValue );
  }

  /**
   * @brief BoseSoundAlert::~BoseSoundAlert
   */
  BoseSoundAlert::~BoseSoundAlert()
  {
    *lg << LDEBUG << "BoseSoundAlert::~BoseSoundAlert: Alert <" << alConfig.getName() << ">, No: <" << BoseSoundAlert::alertCount
        << "> destroing..." << Qt::endl;
    --BoseSoundAlert::alertCount;
  }

  /**
   * @brief BoseSoundAlert::timerEvent
   * @param event
   */
  void BoseSoundAlert::onTimeout()
  {
    //
    // verzweige in die richtige Routine für den jeweiligen Status des Automaten
    //
    switch ( alertStatus )
    {
      case AL_INACTIVE:
        alertStatus = AL_PREPARE1;
        break;
      case AL_PREPARE1:
        // Geräte vorbereiten
        // prepareAlert();
        break;
      case AL_PREPARE2:
        // Standby erfragen
        prepareAlertCheckStandby();
        break;
      case AL_PREPARE3:
        // Lautstärke erfragen
        prepareAlertGetVolume();
        break;
      case AL_PREPARE4:
        // Lautstärke setzen
        prepareAlertSetVolume();
        break;
      case AL_PREPARE5:
        // Quelle einstellen
        prepareAlertSetTuner();
        break;
      case AL_PREPARE6:
        // Abspielen überwachen
        prepareAlertWaitForSource();
        break;
      case AL_RAISE:
        // Lautstärke dimmen
        durationCounter--;
        raiseVolume();
        break;
      case AL_PLAY:
        // nur abspielen
        durationCounter--;
        checkPlaying();
        break;
      case AL_REDUCE:
        // Lautstärke dimmen
        reduceVolume();
        break;
      case AL_POSTPARE:
        // beenden
        postpareAlert();
        break;
      case AL_FINISHED:
        // alert beenden
        alTimer.stop();
        disconnect( &alTimer );
        disconnect( masterDevice.get() );
        emit onFinish();
        break;
      default:
        // ist größer als 0x80 warte ich auf etwas
        if ( alertStatus >= 0x80 )
        {
        }
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
          // in die Liste
          devices.append( avlDevice );
        }
      }  // for availDevices
    }    // for devnames
    //
    // wie viele Geräte gefunden?
    //
    int countDevices = devices.count();
    *lg << LDEBUG << "BoseSoundAlert::prepareAlert: found <" << countDevices << "> device(s) for alert" << Qt::endl;
    if ( countDevices == 0 )
    {
      // kein Gerät!
      *lg << LCRIT << "found no device for alert! abort!" << Qt::endl;
      alertStatus = AL_FINISHED;
      isWorking = false;
      return;
    }
    //
    // Master Device machen
    //
    auto firstDev = devices.first();
    QString ipStr( firstDev.getIp().toString() );
    // auto local_threshold = config->getThreshold() == LgThreshold::LG_DEBUG ? QtMsgType::QtDebugMsg : QtMsgType::QtCriticalMsg;
    masterDevice = sDevicePtr( new BSoundTouchDevice( ipStr, firstDev.getWsPort(), firstDev.getPort(), this ) );
    masterDevice->setId( firstDev.getId() );
    masterDevice->setIpString( firstDev.getIp().toString() );
    //
    // Das Gerät erst mal mit allen Slots verbinden
    //
    if ( config->getThreshold() == LgThreshold::LG_DEBUG )
    {
      connect( masterDevice.get(), &BSoundTouchDevice::sigOnWSConnected,
               [=]() { *( this->lg ) << LG_INFO << "======================== CONNECTED ===================" << Qt::endl; } );
      connect( masterDevice.get(), &BSoundTouchDevice::sigOnWSDisConnected,
               [=]() { *( this->lg ) << LG_INFO << "======================== DISCONNECTED ===================" << Qt::endl; } );
    }
    connect( masterDevice.get(), &BSoundTouchDevice::sigOnSoundTouchEvent, this, &BoseSoundAlert::slotOnSoundTouchEvent );
    //
    // verbinde das Master Gerät
    //
    *lg << LDEBUG << "BoseSoundAlert::prepareAlert: connect Device..." << Qt::endl;
    masterDevice->connectWs();
    //
    // sind noch mehr da?
    //
    if ( countDevices > 1 )
    {
      // TODO:
      // if ( !buildGroup() )
      // {
      //  alertStatus = AL_FINISHED;
      //  *lg << LWARN << "can't create playgroup for alert! try single on master device..." << Qt::endl;
      // single versuchen?
      // master device wir ja trotzdem gesetzt
      // return;
      // }
    }
    //
    // teste ob das Gerät im Standby ist
    //
    askForStandby();
    alertStatus = AL_PREPARE2;  // Status des Automaten
    isWorking = false;
  }

  /**
   * @brief BoseSoundAlert::prepareAlert2
   */
  void BoseSoundAlert::prepareAlertCheckStandby()
  {
    //
    // erwarte den STANDBY Wert
    //
    static bool isWorking = false;
    static quint32 timeoutCount{0};
    if ( isWorking )
      return;
    isWorking = true;
    if ( waitForList.indexOf( WT_STANDBY ) != -1 )
    {
      //
      // ich warte auf STANDBY
      //
      ++timeoutCount;
      if ( timeoutCount > timeoutValue )
      {
        //
        // TIMEOUT!!!!
        //
        *lg << LG_CRIT << "timeout while waitung for standby state for device. playstate is <" << currentPlaystate << ">" << Qt::endl;
        alertStatus = AL_FINISHED;
      }
      isWorking = false;
      return;
    }
    //
    // hier hab ich den Wert
    //
    *lg << LG_DEBUG << "BoseSoundAlert::prepareAlertCheckStandby: standby state found. continue." << Qt::endl;
    isWorking = false;
    timeoutCount = 0;
    askForVolume();
    alertStatus = AL_PREPARE3;
  }

  void BoseSoundAlert::prepareAlertGetVolume()
  {
    //
    // Warte auf Ergebnis der Anfrage nach Lautstärke
    //
    static bool isWorking = false;
    static quint32 timeoutCount{0};
    if ( isWorking )
      return;
    isWorking = true;
    if ( waitForList.indexOf( WT_VOLUME ) != -1 )
    {
      //
      // ich warte auf VOLUME
      //
      ++timeoutCount;
      if ( timeoutCount > timeoutValue )
      {
        //
        // TIMEOUT!!!!
        //
        *lg << LG_CRIT << "timeout while waitung for get volume." << Qt::endl;
        alertStatus = AL_FINISHED;
      }
      isWorking = false;
      return;
    }
    //
    // hier hab ich den Wert
    //
    *lg << LG_DEBUG << "BoseSoundAlert::prepareAlert3: successful got volume <" << currentVolume << ">" << Qt::endl;
    timeoutCount = 0;
    bevorVolume = currentVolume;
    if ( alConfig.getAlRaiseVolume() )
    {
      //
      // Soll die Lautstärke gedimmt werden
      //
      setVolume( 0 );
    }
    else
    {
      //
      // die Lautstärke auf den gewünschten Wert
      //
      setVolume( alertVolume );
    }
    isWorking = false;
    alertStatus = AL_PREPARE4;
  }

  void BoseSoundAlert::prepareAlertSetVolume()
  {
    //
    // warte auf die Einstellung der Lautstärke nach Vorgabe
    //
    static bool isWorking = false;
    if ( isWorking )
      return;
    isWorking = true;
    switch ( waitForSetVolume() )
    {
      case WS_WAITING:
        break;
      case WS_SUCCESS:
        //
        // hier hab ich den Wert
        // nun Sender einstellen
        //
        *lg << LG_DEBUG << "BoseSoundAlert::prepareAlert4: successful set volume <" << currentVolume << ">" << Qt::endl;
        setTunerChannel();
        alertStatus = AL_PREPARE5;
        break;
      case WS_TIMEOUT:
        //
        // TIMEOUT!!!!
        //
        *lg << LG_CRIT << "timeout while waitung for set volume." << Qt::endl;
        alertStatus = AL_FINISHED;
    }
    isWorking = false;
  }

  void BoseSoundAlert::prepareAlertSetTuner()
  {
    //
    // warte auf die Einstellung des Senders/der Quelle
    //
    static bool isWorking = false;
    static quint32 timeoutCount{0};
    if ( isWorking )
      return;
    isWorking = true;
    if ( waitForList.indexOf( WT_SOURCE ) != -1 )
    {
      //
      // ich warte auf Sender Einstellung
      //
      ++timeoutCount;
      if ( timeoutCount > timeoutValue * 2 )
      {
        //
        // TIMEOUT!!!!
        //
        *lg << LG_CRIT << "timeout while waitung for set play source." << Qt::endl;
        alertStatus = AL_FINISHED;
      }
      isWorking = false;
      return;
    }
    //
    // hier hab ich den Wert
    // warten ob der Sender auch spielt
    //
    *lg << LG_DEBUG << "BoseSoundAlert::prepareAlert5: successful set play source <" << currentSource << ">" << Qt::endl;
    timeoutCount = 0;
    if ( currentPlaystate.compare( "PLAY_STATE" ) != 0 )
    {
      //
      // Meldung noch nicht auf abspielen
      //
      if ( waitForList.indexOf( WT_PLAYSTATE ) == -1 )
        waitForList.append( WT_PLAYSTATE );
      isWorking = false;
      alertStatus = AL_PREPARE6;
    }
    else
    {
      //
      // das ging schnell, der sender dudelt
      //
      if ( alConfig.getAlRaiseVolume() )
      {
        alertStatus = AL_RAISE;
      }
      else
      {
        alertStatus = AL_PLAY;
      }
      isWorking = false;
    }
  }

  void BoseSoundAlert::prepareAlertWaitForSource()
  {
    //
    // warte auf das abspielen der Quelle
    //
    static bool isWorking = false;
    static quint32 timeoutCount{0};
    if ( isWorking )
      return;
    isWorking = true;
    if ( waitForList.indexOf( WT_PLAYSTATE ) != -1 )
    {
      //
      // ich warte auf abspielen der quelle
      //
      ++timeoutCount;
      if ( timeoutCount > timeoutValue * 3 )
      {
        //
        // TIMEOUT!!!!
        //
        *lg << LG_CRIT << "timeout while waitung for playstate for source." << Qt::endl;
        alertStatus = AL_FINISHED;
      }
      isWorking = false;
      return;
    }
    //
    // hier hab ich den Wert
    // warten ob der Sender auch spielt
    //
    *lg << LG_DEBUG << "BoseSoundAlert::prepareAlert6: source is playing <" << currentSource << ">" << Qt::endl;
    if ( alConfig.getAlRaiseVolume() )
    {
      alertStatus = AL_RAISE;
    }
    else
    {
      alertStatus = AL_PLAY;
    }
    timeoutCount = 0;
    isWorking = false;
  }

  BoseSoundAlert::waitState BoseSoundAlert::waitForSetVolume()
  {
    //
    // warte auf die Einstellung der Lautstärke nach Vorgabe
    //
    static bool isWorking = false;
    static quint32 timeoutCount{0};
    if ( isWorking )
      return WS_WAITING;
    isWorking = true;
    if ( waitForList.indexOf( WT_VOLUME ) != -1 )
    {
      //
      // ich warte auf VOLUME
      //
      ++timeoutCount;
      if ( timeoutCount > timeoutValue )
      {
        //
        // TIMEOUT!!!!
        //
        *lg << LG_CRIT << "timeout while waitung for set volume." << Qt::endl;
        isWorking = false;
        return WS_TIMEOUT;
      }
      isWorking = false;
      return WS_WAITING;
    }
    //
    // hier hab ich den Wert
    //
    *lg << LG_DEBUG << "BoseSoundAlert::prepareAlert4: successful set volume <" << currentVolume << ">" << Qt::endl;
    timeoutCount = 0;
    isWorking = false;
    return WS_SUCCESS;
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
    switch ( waitForSetVolume() )
    {
      case WS_WAITING:
        break;
      case WS_SUCCESS:
        setVolume( currentVolume + alertRaiseStep );
        break;
      case WS_TIMEOUT:
        alertStatus = AL_FINISHED;
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
    /*
    *lg << LDEBUG << "BoseSoundAlert::buildGroup: found <" << devices.count() << ">, build an group..." << Qt::endl;
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
    */
    return ( true );
  }

  void BoseSoundAlert::slotOnSoundTouchEvent( IResponseObjPtr responsePtr )
  {
    //
    // je nach Art der Meldung...
    //
    switch ( static_cast< qint8 >( responsePtr->getResultType() ) )
    {
      case static_cast< qint8 >( ResultobjectType::R_OK ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: ok recived";
        break;

      case static_cast< qint8 >( ResultobjectType::R_SOURCES ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: resource recived";
        break;

      case static_cast< qint8 >( ResultobjectType::R_BASS_CAPABILITIES ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: bass capabilities recived";
        break;

      case static_cast< qint8 >( ResultobjectType::R_ZONES ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: update presets";
        onZonesResult( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::R_VOLUME ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: volume recived";
        onVolumeResult( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::R_PRESETS ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: presets recived";
        break;

      case static_cast< qint8 >( ResultobjectType::R_BASS ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: bass propertys recived";
        break;

      case static_cast< qint8 >( ResultobjectType::R_DEVICE_INFO ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: device info recived";
        break;

      case static_cast< qint8 >( ResultobjectType::U_PRESETS ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: update presets";
        onPresetsUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::R_NOW_PLAYING ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: update presets";
        onNowPlayingResult( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::R_ERROR ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: emit sigOnErrorResult";
        break;

      case static_cast< qint8 >( ResultobjectType::R_GROUPS ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: emit sigOnGroupResult";
        onGroupUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_NOWPLAYING ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: emit sigOnNowPlayingUpdated";
        onNowPlayingUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_SELECTION ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: emit sigOnPresetSelectionUpdated";
        onPresetSelectionUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_VOLUME ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: emit sigOnVolumeUpdated";
        onVolumeUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_BASS ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: emit sigOnBassUpdated";
        onBassUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_ZONE ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: emit sigOnZoneUpdated";
        onZoneUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_INFO ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: emit sigOnInfoUpdated";
        onInfoUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_NAME ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: emit sigOnNameUpdated";
        onNameUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_ERROR ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: emit sigOnErrorUpdated";
        onErrorUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_GROUP ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: emit sigOnGroupUpdated";
        onGroupUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_SDKINFO ):
      case static_cast< qint8 >( ResultobjectType::U_BROWSE_UNSUPPORTED ):
      case static_cast< qint8 >( ResultobjectType::U_RECENTS_UNSUPPORTED ):
      case static_cast< qint8 >( ResultobjectType::U_SOURCES_UNSUPPORTED ):
      case static_cast< qint8 >( ResultobjectType::U_LANGUAGE_UNSUPPORTED ):
      case static_cast< qint8 >( ResultobjectType::U_USER_ACTIVITY_UPDATED_UNSUPPORTED ):
      case static_cast< qint8 >( ResultobjectType::U_USER_INACTIVITY_UPDATED_UNSUPPORTED ):
      case static_cast< qint8 >( ResultobjectType::U_CONNECTION_STATE_UPDATED_UNSUPPORTED ):
      case static_cast< qint8 >( ResultobjectType::U_AUDIOPRODUCT_TONECONTROLS_UNSUPPORTED ):
      case static_cast< qint8 >( ResultobjectType::U_AUDIOPRODUCT_LEVELCONTROLS_UNSUPPORTED ):
      case static_cast< qint8 >( ResultobjectType::U_AUDIO_SP_CONTROLS_UNSUPPORTED ):
        qDebug() << "BoseSoundAlert::slotOnSoundTouchEvent: unsupported events. ignore...";
        break;

      default:
        qWarning() << "BoseSoundAlert::slotOnSoundTouchEvent: unknown object type. ignore.";
    }
  }

  void BoseSoundAlert::onPresetsUpdated( IResponseObjPtr response )
  {
    *lg << LDEBUG << "BoseSoundAlert::slotOnPresetsUpdated" << Qt::endl;
    WsPresetUpdateObject *pres = static_cast< WsPresetUpdateObject * >( response.get() );
    *lg << LINFO << "on device " << pres->getDeviceId() << " pressed " << pres->getDevicePresets().first().id << " alert aborting"
        << Qt::endl;
    // alertStatus = AL_FINISHED;
  }

  void BoseSoundAlert::onNowPlayingUpdated( IResponseObjPtr response )
  {
    *lg << LDEBUG << "BoseSoundAlert::slotOnNowPlayingUpdated" << Qt::endl;
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
      }
    }
    //
    // speichere den aktuellen Status, soweit noch nicht passiert
    //
    currentPlaystate = npl->getPlayStatus();
    currentSource = npl->getSource();
    //
    *lg << LDEBUG << "BoseSoundAlert::slotOnNowPlayingUpdated: playing status on device " << npl->getDeviceId()
        << " now: " << npl->getPlayStatus() << Qt::endl;
  }

  void BoseSoundAlert::onNowPlayingResult( IResponseObjPtr response )
  {
    *lg << LDEBUG << "BoseSoundAlert::slotOnNowPlayingResult" << Qt::endl;
    HttpNowPlayingObject *npl = static_cast< HttpNowPlayingObject * >( response.get() );
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
      }
    }
    //
    // wartet jemand auf STANDBY
    //
    wasWaiting = waitForList.indexOf( WT_STANDBY );
    if ( wasWaiting != -1 )
    {
      if ( reStandby.exactMatch( npl->getPlayStatus() ) || reStandby.exactMatch( npl->getSource() ) )
      {
        //
        // STANDBY eingetroffen
        //
        currentPlaystate = npl->getPlayStatus();
        waitForList.remove( wasWaiting );
      }
    }
    //
    // speichere den aktuellen Status, soweit noch nicht passiert
    //
    currentPlaystate = npl->getPlayStatus();
    currentSource = npl->getSource();
    //
    *lg << LDEBUG << "BoseSoundAlert::slotOnNowPlayingResult: playing status on device " << npl->getDeviceId()
        << " now: " << npl->getPlayStatus() << " source: " << npl->getSource() << Qt::endl;
  }

  void BoseSoundAlert::onPresetSelectionUpdated( IResponseObjPtr response )
  {
    *lg << LDEBUG << "BoseSoundAlert::slotOnPresetSelectionUpdated" << Qt::endl;
    WsNowSelectionUpdated *nsu = static_cast< WsNowSelectionUpdated * >( response.get() );
    *lg << LDEBUG << "BoseSoundAlert::slotOnPresetSelectionUpdated: preset on device : " << nsu->getDeviceId()
        << " switched to: " << nsu->getDevicePresets().id << Qt::endl;
    // TODO: info benutzen
  }

  void BoseSoundAlert::onVolumeUpdated( IResponseObjPtr response )
  {
    *lg << LDEBUG << "BoseSoundAlert::slotOnVolumeUpdated" << Qt::endl;
    WsVolumeUpdated *vol = static_cast< WsVolumeUpdated * >( response.get() );
    //
    // aktuelle Lautstärke speichern
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
    }
    *lg << LDEBUG << "BoseSoundAlert::slotOnVolumeUpdated: on device : " << vol->getDeviceId()
        << " volume updated to: " << vol->getActualVolume() << Qt::endl;
    // TODO: zur überwachung nutzen
  }

  void BoseSoundAlert::onVolumeResult( IResponseObjPtr response )
  {
    *lg << LDEBUG << "BoseSoundAlert::slotOnVolumeResult" << Qt::endl;
    HttpVolumeObject *vol = static_cast< HttpVolumeObject * >( response.get() );
    //
    // aktuelle Lautstärke speichern
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
    }
    *lg << LDEBUG << "BoseSoundAlert::slotOnVolumeResult: on device : " << vol->getDeviceId()
        << " volume updated to: " << vol->getActualVolume() << Qt::endl;
    // TODO: zur überwachung nutzen
  }

  void BoseSoundAlert::onBassUpdated( IResponseObjPtr response )
  {
    *lg << LDEBUG << "BoseSoundAlert::slotOnBassUpdated" << Qt::endl;
    WsBassUpdated *bu = static_cast< WsBassUpdated * >( response.get() );
    *lg << LDEBUG << "BoseSoundAlert::slotOnBassUpdated: bass updatet on device: " << bu->getDeviceId()
        << " to value: " << bu->getUpdatet() << Qt::endl;
    // Nur INFO
  }

  void BoseSoundAlert::onZoneUpdated( IResponseObjPtr response )
  {
    *lg << LDEBUG << "BoseSoundAlert::slotOnZoneUpdated" << Qt::endl;
    WsZoneUpdated *zu = static_cast< WsZoneUpdated * >( response.get() );
    *lg << LINFO << "zone update on device: " << zu->getDeviceId() << Qt::endl;
    // TODO: zur überwachung der Kommandos
  }

  void BoseSoundAlert::onZonesResult( bose_soundtoch_lib::IResponseObjPtr response )
  {
    *lg << LDEBUG << "BoseSoundAlert::slotOnZonesResult" << Qt::endl;
    HttpZoneObject *zu = static_cast< HttpZoneObject * >( response.get() );
    *lg << LINFO << "zone recived on device: " << zu->getDeviceId() << " master: " << zu->getMaster() << Qt::endl;
    // TODO: zur überwachung der Kommandos
  }

  void BoseSoundAlert::onInfoUpdated( IResponseObjPtr response )
  {
    *lg << LDEBUG << "BoseSoundAlert::slotOnInfoUpdated" << Qt::endl;
    WsInfoUpdated *iu = static_cast< WsInfoUpdated * >( response.get() );
    *lg << LDEBUG << "BoseSoundAlert::slotOnInfoUpdated: on device: " << iu->getDeviceId() << " info update..." << Qt::endl;
    // Nur info
  }

  void BoseSoundAlert::onNameUpdated( IResponseObjPtr response )
  {
    *lg << LDEBUG << "BoseSoundAlert::slotOnNameUpdated" << Qt::endl;
    WsNameUpdated *nu = static_cast< WsNameUpdated * >( response.get() );
    *lg << LDEBUG << "BoseSoundAlert::slotOnNameUpdated: device changed name to: " << nu->getDeviceId() << Qt::endl;
  }

  void BoseSoundAlert::onErrorUpdated( IResponseObjPtr response )
  {
    *lg << LDEBUG << "BoseSoundAlert::slotOnErrorUpdated" << Qt::endl;
    WsErrorUpdated *eu = static_cast< WsErrorUpdated * >( response.get() );
    *lg << LCRIT << "error update on device: " << eu->getError().text << "on device" << eu->getDeviceId();
    // TODO: was machen
  }

  void BoseSoundAlert::onGroupUpdated( IResponseObjPtr response )
  {
    *lg << LDEBUG << "BoseSoundAlert::slotOnGroupUpdated" << Qt::endl;
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
    *lg << LINFO << "BoseSoundAlert::cancelAlert: force to cancel Alert!" << Qt::endl;
    //
    // Alarm beenden!
    //
    //
    // wenn in run() der mutex blockiert ist, entsperre den
    //
    alertStatus = AL_POSTPARE;
  }

  /**
   * @brief BoseSoundAlert::isFinished
   * @return
   */
  bool BoseSoundAlert::isFinished()
  {
    return alertStatus == AL_INACTIVE ? true : false;
  }

  /**
   * @brief BoseSoundAlert::askForVolume blockierend mit timeout
   * @return
   */
  bool BoseSoundAlert::askForVolume()
  {
    *lg << LDEBUG << "BoseSoundAlert::askForVolume: ..." << Qt::endl;
    masterDevice->getVolume();
    //
    // in die Liste das warten auf Lautstärke eintragen, wenn nötig
    //
    if ( waitForList.indexOf( WT_VOLUME ) == -1 )
      waitForList.append( WT_VOLUME );
    *lg << LDEBUG << "BoseSoundAlert::askForVolume: ...OK" << Qt::endl;
    return true;
  }

  /**
   * @brief BoseSoundAlert::askForStandby
   * @return
   */
  bool BoseSoundAlert::askForStandby()
  {
    *lg << LDEBUG << "BoseSoundAlert::askForStandby: ..." << Qt::endl;
    masterDevice->getNowPlaying();
    //
    // in die Liste das warten auf standby eintragen
    //
    if ( waitForList.indexOf( WT_STANDBY ) == -1 )
      waitForList.append( WT_STANDBY );
    *lg << LDEBUG << "BoseSoundAlert::askForStandby: ...OK" << Qt::endl;
    return true;
  }

  bool BoseSoundAlert::setVolume( int vol )
  {
    *lg << LDEBUG << "BoseSoundAlert::setVolume: set to: " << vol << "..." << Qt::endl;
    masterDevice->setVolume( vol );
    //
    // in die Liste das warten auf Lautstärke eintragen, wenn nötig
    //
    if ( waitForList.indexOf( WT_VOLUME ) == -1 )
      waitForList.append( WT_VOLUME );
    *lg << LDEBUG << "BoseSoundAlert::setVolume: ...OK" << Qt::endl;
    return true;
  }

  bool BoseSoundAlert::setTunerChannel()
  {
    QString source = alConfig.getAlSource();
    *lg << LDEBUG << "BoseSoundAlert::setTunerChannel: set to: " << source << "..." << Qt::endl;
    //
    // welche Art Source
    //
    // "^PRESET_[123456]$"
    // "INTERNET_RADIO|TUNEIN"
    // "^AMAZON$"
    // "^STANDBY$"

    if ( rePreset.exactMatch( source ) )
    {
      *lg << LINFO << "playing PRESET <" << source << ">..." << Qt::endl;
      //
      // es soll ein PRESET gespielt werden, finde die Nummer heraus
      //
      int captured = rePresetNum.indexIn( source );
      if ( captured >= 0 )
      {
        //
        // da war was...
        // die Nummer des Preset
        //
        int preset = rePresetNum.capturedTexts()[ 1 ].toInt();
        *lg << LDEBUG << "BoseSoundAlert::setTunerChannel: Number of PRESET: <" << preset << ">..." << Qt::endl;
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
            *lg << LWARN << "preset not valid: <" << preset << ">" << Qt::endl;
        }
        *lg << LDEBUG << "BoseSoundAlert::setTunerChannel: send keypress to master device..." << Qt::endl;
        masterDevice->setKey( whichkey, BSoundTouchDevice::bose_keystate::KEY_RELEASED );
      }
      else
      {
        *lg << LWARN << "can't find which preset you want. set was <" << captured << ">. try default PRESET_1...";
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
    }
    *lg << LDEBUG << "BoseSoundAlert::setTunerChannel: ...OK" << Qt::endl;
    return true;
  }

}  // namespace bose_commserver
