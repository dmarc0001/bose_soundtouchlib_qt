#include "librarytestwindow.hpp"

namespace radio
{
  LibraryTestWindow::LibraryTestWindow( QString hostname, int wsPortnum, int httpPortnum, bool isDebug, QWidget *parent )
      : QMainWindow( parent )
      , host( hostname )
      , wsPort( wsPortnum )
      , httpPort( httpPortnum )
      , isDebug( isDebug )
      , ui( new Ui::LibraryTestWindow() )
  {
    ui.get()->setupUi( this );
    ui->hostnameLineEdit->setText( host );
    //
    // Logging
    //
    lg = std::shared_ptr< Logger >( new Logger() );
    if ( isDebug )
    {
      //
      // loggen mit debug
      //
      lg->startLogging( LG_DEBUG, QString( ProgramConfig::logfile ) );
    }
    else
    {
      //
      // alles ab info loggen
      //
      lg->startLogging( LG_INFO, QString( ProgramConfig::logfile ) );
    }
    lg->debug( "TestMainWindow::TestMainWindow" );
    sDevice = std::unique_ptr< BSoundTouchDevice >( new BSoundTouchDevice( host, httpPort, wsPort, lg, this ) );
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
  }

  LibraryTestWindow::~LibraryTestWindow()
  {
    lg->debug( "LibraryTestWindow::~LibraryTestWindow..." );
    lg->shutdown();
  }

  void LibraryTestWindow::slotOnHostnameLineEditFinished( void )
  {
    lg->debug( QString( "LibraryTestWindow::slotOnHostnameLineEditFinished: %1" ).arg( ui->hostnameLineEdit->text() ) );
    host = ui->hostnameLineEdit->text();
    sDevice->setHostname( host );
  }

  void LibraryTestWindow::slotOnConnectWsButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnConnectButton..." );
  }

  void LibraryTestWindow::slotOnDisconnectWsButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnDisconnectButton..." );
  }

  void LibraryTestWindow::slotOnVolumeCheckButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnVolumeCheckButton..." );
    sDevice->getVolume();
  }

  void LibraryTestWindow::slotOnSourcesListButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnSourcesListButton..." );
    sDevice->getSources();
  }

  void LibraryTestWindow::slotOnBassCapabilitiesButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnBassCapabilitiesButton..." );
    sDevice->getBassCapabilities();
  }

  void LibraryTestWindow::slotOnBassButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnBassButton..." );
    sDevice->getBass();
  }

  void LibraryTestWindow::slotOnGetZoneButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnGetZoneButton..." );
    sDevice->getZone();
  }

  void LibraryTestWindow::slotNowPlayingButton( void )
  {
    lg->debug( "LibraryTestWindow::slotNowPlayingButton..." );
    sDevice->getNowPlaying();
  }

  void LibraryTestWindow::slotOnGetPresetsButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnGetPresetsButton..." );
    sDevice->getPresets();
  }

  void LibraryTestWindow::slotOnGetDeviceInfoButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnGetDeviceInfoButton..." );
    sDevice->getDeviceInfo();
  }

  void LibraryTestWindow::slotOnPlayButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnPlayButton..." );
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_PLAY, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnPauseButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnPauseButton..." );
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_PAUSE, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnPreviousButon( void )
  {
    lg->debug( "LibraryTestWindow::slotOnPreviousButon..." );
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_PREV_TRACK, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnNextButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnNextButton..." );
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_NEXT_TRACK, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnMuteButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnMuteButton..." );
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_MUTE, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnShuffleOnButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnShuffleOnButton..." );
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_SHUFFLE_ON, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnShuffleOffButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnShuffleOffButton..." );
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_SHUFFLE_OFF, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnRepeatOneButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnRepeatOneButton..." );
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_REPEAT_ONE, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnRepeatAllButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnRepeatAllButton..." );
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_REPEAT_ALL, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnRepeatOffButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnRepeatOffButton..." );
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_REPEAT_OFF, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnAddFavoriteButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnAddFavoriteButton..." );
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_ADD_FAVORITE, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnRemoveFavoriteButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnRemoveFavoriteButton..." );
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_REMOVE_FAVORITE, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnThumbsUpButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnThumbsUpButton..." );
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_THUMBS_UP, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnThumbsDownButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnThumbsDownButton..." );
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_THUMBS_DOWN, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnBookmarkButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnBookmarkButton..." );
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_BOOKMARK, BSoundTouchDevice::bose_keystate::KEY_PRESSED );
  }

  void LibraryTestWindow::slotOnPowerButton( void )
  {
    lg->debug( "LibraryTestWindow::slotOnPowerButton..." );
    sDevice->setKey( BSoundTouchDevice::bose_key::KEY_POWER, BSoundTouchDevice::bose_keystate::KEY_TOGGLE );
  }

  void LibraryTestWindow::slotOnPresetButton( int preset )
  {
    lg->debug( QString( "LibraryTestWindow::slotOnPresetButton: pressed: %1" ).arg( preset ) );
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
    lg->debug( QString( "LibraryTestWindow::slotOnPresetButton: send: %1" )
                   .arg( BSoundTouchDevice::keynames[ static_cast< int >( whichkey ) ] ) );
    sDevice->setKey( whichkey, BSoundTouchDevice::bose_keystate::KEY_RELEASED );
  }
}  // namespace radio
