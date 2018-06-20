#ifndef LIBRARYTESTWINDOW_HPP
#define LIBRARYTESTWINDOW_HPP

#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QMainWindow>
#endif
#include <memory>
#include "bsoundtouchdevice.hpp"
#include "config/programconfig.hpp"
#include "logging/Logger.hpp"
#include "ui_librarytestwindow.h"

namespace Ui
{
  class LibraryTestWindow;
}

namespace radio
{
  class LibraryTestWindow : public QMainWindow
  {
    Q_OBJECT
    private:
    QString host;
    int wsPort;
    int httpPort;
    bool isDebug;
    std::unique_ptr< Ui::LibraryTestWindow > ui;
    std::shared_ptr< Logger > lg;
    std::unique_ptr< BSoundTouchDevice > sDevice;

    public:
    explicit LibraryTestWindow( QString hostname, int wsPortnum, int httpPortnum, bool isDebug, QWidget *parent = nullptr );
    ~LibraryTestWindow();

    private:
    private slots:
    void slotOnConnectWsButton( void );
    void slotOnDisconnectWsButton( void );
    // hostnameedit
    void slotOnHostnameLineEditFinished( void );
    // get Funktionen
    void slotOnVolumeCheckButton( void );
    void slotOnSourcesListButton( void );
    void slotOnBassCapabilitiesButton( void );
    void slotOnBassButton( void );
    void slotOnGetZoneButton( void );
    void slotNowPlayingButton( void );
    void slotOnGetPresetsButton( void );
    void slotOnGetDeviceInfoButton( void );
    // keypress
    void slotOnPlayButton( void );
    void slotOnPauseButton( void );
    void slotOnPreviousButon( void );
    void slotOnNextButton( void );
    void slotOnMuteButton( void );
    void slotOnShuffleOnButton( void );
    void slotOnShuffleOffButton( void );
    void slotOnRepeatOneButton( void );
    void slotOnRepeatAllButton( void );
    void slotOnRepeatOffButton( void );
    void slotOnAddFavoriteButton( void );
    void slotOnRemoveFavoriteButton( void );
    void slotOnThumbsUpButton( void );
    void slotOnThumbsDownButton( void );
    void slotOnBookmarkButton( void );
    void slotOnPowerButton( void );
    void slotOnPresetButton( int preset );
  };
}  // namespace radio
#endif  // LIBRARYTESTWINDOW_HPP
