#ifndef LIBRARYTESTWINDOW_HPP
#define LIBRARYTESTWINDOW_HPP

#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QMainWindow>
#endif
#include <QtDebug>
#include <bsoundtouchdevice.hpp>
#include <iostream>
#include <memory>
#include "config/programconfig.hpp"
#include "ui_librarytestwindow.h"
#include "xmlparser/xmlupdateparser.hpp"

namespace Ui
{
  class LibraryTestWindow;
}

namespace bose_soundtoch_lib
{
  class LibraryTestWindow : public QMainWindow
  {
    Q_OBJECT
    private:
    QString host;
    quint16 wsPort;
    quint16 httpPort;
    bool isDebug;
    QtMsgType threshold;
    std::unique_ptr< Ui::LibraryTestWindow > ui;
    std::unique_ptr< BSoundTouchDevice > sDevice;

    public:
    explicit LibraryTestWindow( QString hostname, int wsPortnum, int httpPortnum, bool isDebug, QWidget *parent = nullptr );
    ~LibraryTestWindow();

    private:
    // static void myMessageOutput( QtMsgType type, const QMessageLogContext &context, const QString &msg );

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
    void slotOnGetGroupButton( void );
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
    //
    // "callbacks" für asyncrone ereignisse
    //
    void slotOnPresetsUpdated( std::shared_ptr< IResponseObject > response );
    void slotOnNowPlayingUpdated( std::shared_ptr< IResponseObject > response );
    void slotOnPresetSelectionUpdated( std::shared_ptr< IResponseObject > response );
    void slotOnVolumeUpdated( std::shared_ptr< IResponseObject > response );
    void slotOnBassUpdated( std::shared_ptr< IResponseObject > response );
    void slotOnZoneUpdated( std::shared_ptr< IResponseObject > response );
    void slotOnInfoUpdated( std::shared_ptr< IResponseObject > response );
    void slotOnNameUpdated( std::shared_ptr< IResponseObject > response );
    void slotOnErrorUpdated( std::shared_ptr< IResponseObject > response );
    void slotOnGroupUpdated( std::shared_ptr< IResponseObject > response );
  };
}  // namespace bose_soundtoch_lib
#endif  // LIBRARYTESTWINDOW_HPP
