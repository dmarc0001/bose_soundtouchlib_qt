#include "librarytestwindow.hpp"

namespace bose_soundtoch_lib
{
  LibraryTestWindow::LibraryTestWindow( QString hostname, int wsPortnum, int httpPortnum, bool isDebug, QWidget *parent )
      : QMainWindow( parent )
      , host( hostname )
      , wsPort( static_cast< quint16 >( wsPortnum ) )
      , httpPort( static_cast< quint16 >( httpPortnum ) )
      , isDebug( isDebug )
      , threshold( QtMsgType::QtInfoMsg )
      , ui( new Ui::LibraryTestWindow() )
  {
    ui.get()->setupUi( this );
    ui->hostnameLineEdit->setText( host );
    //
    // Logging
    // macht die library...
    // qInstallMessageHandler( &LibraryTestWindow::myMessageOutput );

    if ( isDebug )
    {
      //
      // loggen mit debug
      //
      threshold = QtMsgType::QtDebugMsg;
      qDebug() << "Logging DEBUGMODE";
    }
    sDevice = std::unique_ptr< BSoundTouchDevice >( new BSoundTouchDevice( host, wsPort, httpPort, this, threshold ) );
    //
    // connect slots mit signalen
    // hostname editiert
    //
    connect( ui->hostnameLineEdit, &QLineEdit::editingFinished, this, &LibraryTestWindow::slotOnHostnameLineEditFinished );
    // GET Funktionen
    connect( ui->connectWsPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnConnectWsButton );
    connect( ui->disconnectWsPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnDisconnectWsButton );
    connect( ui->volumePushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnVolumeCheckButton );
    connect( ui->sourcesPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnSourcesListButton );
    connect( ui->bassCapabilitiesPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnBassCapabilitiesButton );
    connect( ui->bassPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnBassButton );
    connect( ui->getZonePushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnGetZoneButton );
    connect( ui->nowPlayingPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotNowPlayingButton );
    connect( ui->presetsPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnGetPresetsButton );
    connect( ui->devInfoPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnGetDeviceInfoButton );
    connect( ui->getGroupPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnGetGroupButton );
    //
    // KEYPRESS (POST)
    //
    connect( ui->powerTogglePushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnPowerButton );
    connect( ui->playPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnPlayButton );
    connect( ui->pausePushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnPauseButton );
    connect( ui->previousPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnPreviousButon );
    connect( ui->nextPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnNextButton );
    connect( ui->mutePushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnMuteButton );
    connect( ui->shuffleOnPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnShuffleOnButton );
    connect( ui->shuffleOffPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnShuffleOffButton );
    connect( ui->repeatOnePushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnRepeatOneButton );
    connect( ui->repeateAllPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnRepeatAllButton );
    connect( ui->repeatOffPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnRepeatOffButton );
    connect( ui->addFavoritePushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnAddFavoriteButton );
    connect( ui->removeFavoritePushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnRemoveFavoriteButton );
    connect( ui->thumbsUpPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnThumbsUpButton );
    connect( ui->thumbsDownPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnThumbsDownButton );
    connect( ui->bookmarkPushButton, &QPushButton::clicked, this, &LibraryTestWindow::slotOnBookmarkButton );
    // presetbuttons via lambda
    connect( ui->preset01PushButton, &QPushButton::clicked, this, [=] { slotOnPresetButton( 0 ); } );
    connect( ui->preset02PushButton, &QPushButton::clicked, this, [=] { slotOnPresetButton( 1 ); } );
    connect( ui->preset03PushButton, &QPushButton::clicked, this, [=] { slotOnPresetButton( 2 ); } );
    connect( ui->preset04PushButton, &QPushButton::clicked, this, [=] { slotOnPresetButton( 3 ); } );
    connect( ui->preset05PushButton, &QPushButton::clicked, this, [=] { slotOnPresetButton( 4 ); } );
    connect( ui->preset06PushButton, &QPushButton::clicked, this, [=] { slotOnPresetButton( 5 ); } );
    //
    // callbacks für ereignisse vom gerät
    //
    connect( sDevice.get(), &BSoundTouchDevice::sigOnSoundTouchEvent, this, &LibraryTestWindow::slotOnSoundTouchEvent );
    connect( sDevice.get(), &BSoundTouchDevice::sigOnWSConnected, this, &LibraryTestWindow::slotOnWSConnected );
    connect( sDevice.get(), &BSoundTouchDevice::sigOnWSDisConnected, this, &LibraryTestWindow::slotOnWSDisConnected );
    /*
    connect( sDevice.get(), &BSoundTouchDevice::sigOnPresetsUpdated, this, &LibraryTestWindow::slotOnPresetsUpdated );
    connect( sDevice.get(), &BSoundTouchDevice::sigOnNowPlayingUpdated, this, &LibraryTestWindow::slotOnNowPlayingUpdated );
    connect( sDevice.get(), &BSoundTouchDevice::sigOnPresetSelectionUpdated, this, &LibraryTestWindow::slotOnPresetSelectionUpdated );
    connect( sDevice.get(), &BSoundTouchDevice::sigOnVolumeUpdated, this, &LibraryTestWindow::slotOnVolumeUpdated );
    connect( sDevice.get(), &BSoundTouchDevice::sigOnBassUpdated, this, &LibraryTestWindow::slotOnBassUpdated );
    connect( sDevice.get(), &BSoundTouchDevice::sigOnZoneUpdated, this, &LibraryTestWindow::slotOnZoneUpdated );
    connect( sDevice.get(), &BSoundTouchDevice::sigOnInfoUpdated, this, &LibraryTestWindow::slotOnInfoUpdated );
    connect( sDevice.get(), &BSoundTouchDevice::sigOnNameUpdated, this, &LibraryTestWindow::slotOnNameUpdated );
    connect( sDevice.get(), &BSoundTouchDevice::sigOnErrorUpdated, this, &LibraryTestWindow::slotOnErrorUpdated );
    connect( sDevice.get(), &BSoundTouchDevice::sigOnGroupUpdated, this, &LibraryTestWindow::slotOnGroupUpdated );
    */
  }

  LibraryTestWindow::~LibraryTestWindow()
  {
    qDebug() << "...LibraryTestWindow::~LibraryTestWindow...";
  }

  void LibraryTestWindow::slotOnHostnameLineEditFinished( void )
  {
    qDebug() << "editet hostname: " << ui->hostnameLineEdit->text();
    host = ui->hostnameLineEdit->text();
    sDevice->setHostname( host );
  }

  void LibraryTestWindow::slotOnConnectWsButton( void )
  {
    qDebug() << "...";
    sDevice->connectWs();
    // sDevice->addVolumeListener();
  }

  void LibraryTestWindow::slotOnDisconnectWsButton( void )
  {
    qDebug() << "...";
    sDevice->disconnectWs();
  }

  void LibraryTestWindow::slotOnVolumeCheckButton( void )
  {
    qDebug() << "...";
    sDevice->getVolume();
  }

  void LibraryTestWindow::slotOnSourcesListButton( void )
  {
    qDebug() << "...";
    sDevice->getSources();
  }

  void LibraryTestWindow::slotOnBassCapabilitiesButton( void )
  {
    qDebug() << "...";
    sDevice->getBassCapabilities();
  }

  void LibraryTestWindow::slotOnBassButton( void )
  {
    qDebug() << "...";
    sDevice->getBass();
  }

  void LibraryTestWindow::slotOnGetZoneButton( void )
  {
    qDebug() << "...";
    sDevice->getZone();
  }

  void LibraryTestWindow::slotNowPlayingButton( void )
  {
    qDebug() << "...";
    sDevice->getNowPlaying();
  }

  void LibraryTestWindow::slotOnGetPresetsButton( void )
  {
    qDebug() << "...";
    sDevice->getPresets();
  }

  void LibraryTestWindow::slotOnGetDeviceInfoButton( void )
  {
    qDebug() << "...";
    sDevice->getDeviceInfo();
  }

  void LibraryTestWindow::slotOnGetGroupButton( void )
  {
    qDebug() << "...";
    sDevice->getGroup();
  }
  void LibraryTestWindow::slotOnPlayButton( void )
  {
    qDebug() << "...";
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_PLAY, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnPauseButton( void )
  {
    qDebug() << "...";
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_PAUSE, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnPreviousButon( void )
  {
    qDebug() << "...";
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_PREV_TRACK, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnNextButton( void )
  {
    qDebug() << "...";
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_NEXT_TRACK, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnMuteButton( void )
  {
    qDebug() << "...";
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_MUTE, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnShuffleOnButton( void )
  {
    qDebug() << "...";
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_SHUFFLE_ON, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnShuffleOffButton( void )
  {
    qDebug() << "...";
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_SHUFFLE_OFF, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnRepeatOneButton( void )
  {
    qDebug() << "...";
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_REPEAT_ONE, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnRepeatAllButton( void )
  {
    qDebug() << "...";
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_REPEAT_ALL, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnRepeatOffButton( void )
  {
    qDebug() << "...";
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_REPEAT_OFF, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnAddFavoriteButton( void )
  {
    qDebug() << "...";
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_ADD_FAVORITE, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnRemoveFavoriteButton( void )
  {
    qDebug() << "...";
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_REMOVE_FAVORITE, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnThumbsUpButton( void )
  {
    qDebug() << "...";
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_THUMBS_UP, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnThumbsDownButton( void )
  {
    qDebug() << "...";
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_THUMBS_DOWN, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnBookmarkButton( void )
  {
    qDebug() << "...";
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_BOOKMARK, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnPowerButton( void )
  {
    qDebug() << "...";
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_POWER, BSoundTouchDevice::bose_keystate::KEY_TOGGLE );
  }

  void LibraryTestWindow::slotOnPresetButton( int preset )
  {
    qDebug() << "pressed: " << preset;
    BSoundTouchDevice::bose_key whichkey;
    //
    // nur "release", "pressed" setzt den preset neu
    //
    switch ( preset )
    {
      case 0:
      default:
        whichkey = BSoundTouchDevice::bose_key::KEY_PRESET_1;
        break;

      case 1:
        whichkey = BSoundTouchDevice::bose_key::KEY_PRESET_2;
        break;

      case 2:
        whichkey = BSoundTouchDevice::bose_key::KEY_PRESET_3;
        break;

      case 3:
        whichkey = BSoundTouchDevice::bose_key::KEY_PRESET_4;
        break;

      case 4:
        whichkey = BSoundTouchDevice::bose_key::KEY_PRESET_5;
        break;

      case 5:
        whichkey = BSoundTouchDevice::bose_key::KEY_PRESET_6;
        break;
    }
    //
    // jetzt den richtigen button senden (release ist "abspielen")
    //
    qDebug() << "send: " << sDevice->getKeyName( whichkey );
    sDevice->setKey( whichkey, BSoundTouchDevice::bose_keystate::KEY_RELEASED );
  }

  void LibraryTestWindow::slotOnWSConnected()
  {
    qDebug() << "connected.";
  }

  void LibraryTestWindow::slotOnWSDisConnected()
  {
    qDebug() << "disconnected.";
  }

  void LibraryTestWindow::slotOnSoundTouchEvent( IResponseObjPtr responsePtr )
  {
    switch ( static_cast< qint8 >( responsePtr->getResultType() ) )
    {
      case static_cast< qint8 >( ResultobjectType::R_OK ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: ok recived";
        break;

      case static_cast< qint8 >( ResultobjectType::R_SOURCES ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: resource recived";
        break;

      case static_cast< qint8 >( ResultobjectType::R_BASS_CAPABILITIES ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: bass capabilities recived";
        break;

      case static_cast< qint8 >( ResultobjectType::R_ZONES ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: update presets";
        break;

      case static_cast< qint8 >( ResultobjectType::R_VOLUME ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: volume recived";
        break;

      case static_cast< qint8 >( ResultobjectType::R_PRESETS ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: presets recived";
        break;

      case static_cast< qint8 >( ResultobjectType::R_BASS ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: bass propertys recived";
        break;

      case static_cast< qint8 >( ResultobjectType::R_DEVICE_INFO ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: device info recived";
        break;

      case static_cast< qint8 >( ResultobjectType::U_PRESETS ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: update presets";
        onPresetsUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::R_NOW_PLAYING ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: update presets";
        break;

      case static_cast< qint8 >( ResultobjectType::R_ERROR ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: emit sigOnErrorResult";
        break;

      case static_cast< qint8 >( ResultobjectType::R_GROUPS ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: emit sigOnGroupResult";
        break;

      case static_cast< qint8 >( ResultobjectType::U_NOWPLAYING ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: emit sigOnNowPlayingUpdated";
        onNowPlayingUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_SELECTION ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: emit sigOnPresetSelectionUpdated";
        onPresetSelectionUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_VOLUME ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: emit sigOnVolumeUpdated";
        onVolumeUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_BASS ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: emit sigOnBassUpdated";
        onBassUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_ZONE ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: emit sigOnZoneUpdated";
        onZoneUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_INFO ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: emit sigOnInfoUpdated";
        onInfoUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_NAME ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: emit sigOnNameUpdated";
        onNameUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_ERROR ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: emit sigOnErrorUpdated";
        onErrorUpdated( responsePtr );
        break;

      case static_cast< qint8 >( ResultobjectType::U_GROUP ):
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: emit sigOnGroupUpdated";
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
        qDebug() << "LibraryTestWindow::slotOnSoundTouchEvent: unsupported events. ignore...";
        break;

      default:
        qWarning() << "LibraryTestWindow::slotOnSoundTouchEvent: unknown object type. ignore.";
    }
  }

  void LibraryTestWindow::onPresetsUpdated( std::shared_ptr< IResponseObject > response )
  {
    WsPresetUpdateObject *pres = static_cast< WsPresetUpdateObject * >( response.get() );
    qInfo() << "LibraryTestWindow::slotOnPresetsUpdated: Preset's first id: " << pres->getDevicePresets().first().id << "on device "
            << pres->getDeviceId();
  }

  void LibraryTestWindow::onNowPlayingUpdated( std::shared_ptr< IResponseObject > response )
  {
    WsNowPlayingUpdate *npl = static_cast< WsNowPlayingUpdate * >( response.get() );
    qInfo() << "LibraryTestWindow::slotOnNowPlayingUpdated: now playing status: " << npl->getPlayStatus() << "on device "
            << npl->getDeviceId();
  }

  void LibraryTestWindow::onPresetSelectionUpdated( std::shared_ptr< IResponseObject > response )
  {
    WsNowSelectionUpdated *nsu = static_cast< WsNowSelectionUpdated * >( response.get() );
    qInfo() << "LibraryTestWindow::slotOnPresetSelectionUpdated: switch to preset id: " << nsu->getDevicePresets().id << "on device "
            << nsu->getDeviceId();
  }

  void LibraryTestWindow::onVolumeUpdated( std::shared_ptr< IResponseObject > response )
  {
    WsVolumeUpdated *vol = static_cast< WsVolumeUpdated * >( response.get() );
    qInfo() << "LibraryTestWindow::slotOnVolumeUpdated: Volume set to: " << vol->getActualVolume() << "on device "
            << vol->getDeviceId();
  }

  void LibraryTestWindow::onBassUpdated( std::shared_ptr< IResponseObject > response )
  {
    WsBassUpdated *bu = static_cast< WsBassUpdated * >( response.get() );
    qInfo() << "LibraryTestWindow::slotOnBassUpdated: bass updated, value" << bu->getUpdatet() << "on device " << bu->getDeviceId();
  }

  void LibraryTestWindow::onZoneUpdated( std::shared_ptr< IResponseObject > response )
  {
    WsZoneUpdated *zu = static_cast< WsZoneUpdated * >( response.get() );
    qInfo() << "LibraryTestWindow::slotOnZoneUpdated: zone update on device: " << zu->getDeviceId();
  }

  void LibraryTestWindow::onInfoUpdated( std::shared_ptr< IResponseObject > response )
  {
    WsInfoUpdated *iu = static_cast< WsInfoUpdated * >( response.get() );
    qInfo() << "LibraryTestWindow::slotOnInfoUpdated: info update on device: " << iu->getDeviceId();
  }

  void LibraryTestWindow::onNameUpdated( std::shared_ptr< IResponseObject > response )
  {
    WsNameUpdated *nu = static_cast< WsNameUpdated * >( response.get() );
    qInfo() << "LibraryTestWindow::slotOnNameUpdated: name update on device: " << nu->getDeviceId();
  }

  void LibraryTestWindow::onErrorUpdated( std::shared_ptr< IResponseObject > response )
  {
    WsErrorUpdated *eu = static_cast< WsErrorUpdated * >( response.get() );
    qInfo() << "LibraryTestWindow::slotOnErrorUpdated: error update on device: " << eu->getError().text << "on device"
            << eu->getDeviceId();
  }

  void LibraryTestWindow::onGroupUpdated( std::shared_ptr< IResponseObject > response )
  {
    WsGroupUpdated *gu = static_cast< WsGroupUpdated * >( response.get() );
    qInfo() << "LibraryTestWindow::slotOnGroupUpdated: group update on device: " << gu->getDeviceId();
  }

  /*
  void LibraryTestWindow::myMessageOutput( QtMsgType type, const QMessageLogContext &context, const QString &msg )
  {
    QByteArray localMsg = msg.toLocal8Bit();
    QStringList functParts;
    QString func;
    // const char *fname;
    switch ( type )
    {
      case QtDebugMsg:
        //
        // versuche nur objekt/function
        //
        functParts = QString( context.function ).split( "::" );
        if ( functParts.count() > 2 )
        {
          func = QString( functParts.value( functParts.count() - 2 ) )
                     .append( "::" )
                     .append( functParts.value( functParts.count() - 1 ) );
        }
        else
          func = context.function;
        fprintf( stdout, "DEBUG: %s: %s\n", func.toStdString().c_str(), localMsg.constData() );
        // fprintf( stdout, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
      case QtInfoMsg:
        fprintf( stdout, "INFO: %s\n", localMsg.constData() );
        // fprintf( stdout, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
      case QtWarningMsg:
        fprintf( stdout, "WARNING: %s\n", localMsg.constData() );
        // fprintf( stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
      case QtCriticalMsg:
        fprintf( stdout, "CRITICAL: %s (%s:%u)\n", localMsg.constData(), context.file, context.line );
        // fprintf( stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
      case QtFatalMsg:
        fprintf( stdout, "FATAL: %s (%s:%u)\n", localMsg.constData(), context.file, context.line );
        // fprintf( stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
    }
  }
  */

}  // namespace bose_soundtoch_lib
