#ifndef BSOUNDTOUCHDEVICE_HPP
#define BSOUNDTOUCHDEVICE_HPP

#include <stdio.h>
#include <QAuthenticator>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QPair>
#include <QVector>
#include <QtDebug>
#include <memory>
#include "websocket/bwebsocket.hpp"
#include "xmlparser/bsoundtouch_global.hpp"
#include "xmlparser/iresponseobject.hpp"

#if defined( SOUNDTOUCH_QT_LIB_LIBRARY )
#define SOUNDTOUCH_QT_LIBSHARED_EXPORT Q_DECL_EXPORT
#else
#define SOUNDTOUCH_QT_LIBSHARED_EXPORT Q_DECL_IMPORT
#endif

// bose developer dmarc0001
// pw dd03353e83

namespace bose_soundtoch_lib
{
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT BSoundTouchDevice : public QObject
  {
    Q_OBJECT

    public:
    //! private interne Klasse möglichkeiten der keys
    enum class bose_keystate : int
    {
      KEY_PRESSED,
      KEY_RELEASED,
      KEY_TOGGLE,
      KEY_UNKNOWN
    };
    //! Aufzählung der (virtuellen) Tasten
    enum class bose_key : int
    {
      KEY_PLAY,
      KEY_PAUSE,
      KEY_PLAYPAUSE,
      KEY_PREV_TRACK,
      KEY_NEXT_TRACK,
      KEY_MUTE,
      KEY_SHUFFLE_ON,
      KEY_SHUFFLE_OFF,
      KEY_REPEAT_ONE,
      KEY_REPEAT_ALL,
      KEY_REPEAT_OFF,
      KEY_ADD_FAVORITE,
      KEY_REMOVE_FAVORITE,
      KEY_THUMBS_UP,
      KEY_THUMBS_DOWN,
      KEY_BOOKMARK,
      KEY_PRESET_1,
      KEY_PRESET_2,
      KEY_PRESET_3,
      KEY_PRESET_4,
      KEY_PRESET_5,
      KEY_PRESET_6,
      KEY_POWER,
      KEY_UNKNOWN
    };

    private:
    QString hostname;
    qint16 wsPort;
    qint16 httpPort;
    std::unique_ptr< BWebsocket > webSocket;
    QtMsgType threshold;
    QUrl url;
    QNetworkAccessManager qnam;
    // QNetworkReply *reply;
    static const QMap< bose_keystate, QString > keystati;
    static const QMap< bose_key, QString > keynames;  //! Namen der Tasten
    static const char *subproto;
    constexpr static int timeoutMillis = 80;
    static const QString version;

    public:
    explicit BSoundTouchDevice( QString &stHost,
                                qint16 stWSPort,
                                qint16 stHttpPort,
                                QObject *parent = nullptr,
                                QtMsgType sth = QtMsgType::QtFatalMsg );
    ~BSoundTouchDevice();
    void setHostname( const QString &stHost );
    void setWSPort( qint16 stWSPort );
    void setHttpPort( qint16 stHttpPort );
    void getSources( void );
    void getBassCapabilities( void );
    void getBass( void );
    void getZone( void );
    void getNowPlaying( void );
    void getTrackInfo( void );
    void getVolume( void );
    void getPresets( void );
    void getDeviceInfo( void );
    void getGroup( void );  //! Soundtouch 10 only
    QString getKeyName( bose_key key ) const;
    QString getKeyStateName( bose_keystate state ) const;
    void setKey( bose_key whichkey, bose_keystate keystate, QString sender = "Gabbo" );       // POST
    void selectSource( const QString &source, const QString &account );                       //! AUX/AMAZON/INTERNET etc...
    void setBass( int bass );                                                                 // POST
    void setVolume( int volume );                                                             // POST
    void setZone( const QString &masterId, const SoundTouchMemberList &memberList );          //! create zone
    void addZoneSlave( const QString &masterId, const SoundTouchMemberList &memberList );     // POST
    void removeZoneSlave( const QString &masterId, const SoundTouchMemberList &memberList );  // POST
    void setDeviceName( QString &name );                                                      // POST
    // websocket überwachung
    void addVolumeListener( void );
    QString getVersion() const;
    void connectWs( void );     //! verbinde gerät mit Websocket, falls noch nciht geschehen
    void disconnectWs( void );  //! trenne WS

    private:
    void startGetRequest( const QUrl &requested );
    void startGetRequest( const QString &requestedUrl );
    void startGetRequest( const char *requested );
    void startGetRequest( void );
    void startPostRequest( const QUrl &requested, const QString &data );
    void startPostRequest( const char *requested, const QString &data );
    void startPostRequest( const QString &data );
    void setUrl( const char *path );
    void editZone( const QString &masterId, const SoundTouchMemberList &memberList );  //! je url create,add,remove
    static void myMessageOutput( QtMsgType type, const QMessageLogContext &context, const QString &msg );

    signals:
    void sigAuthenticationRequired( QNetworkReply *repl, QAuthenticator *authenticator );
    void sigOnWSConnected( void );
    void sigOnWSDisConnected( void );
    void sigOnWSTextMessageReceived( QString message );
    //
    void sigOnPresetsUpdated( std::shared_ptr< IResponseObject > );          //! sig on preset Update
    void sigOnNowPlayingUpdated( std::shared_ptr< IResponseObject > );       //! sig on now playing Update
    void sigOnPresetSelectionUpdated( std::shared_ptr< IResponseObject > );  //! sig on selction an Update
    void sigOnVolumeUpdated( std::shared_ptr< IResponseObject > );           //! sig on volume changed/updated
    void sigOnBassUpdated( std::shared_ptr< IResponseObject > );             //! sig on bass property updated
    void sigOnZoneUpdated( std::shared_ptr< IResponseObject > );             //! sig on zone updated
    void sigOnInfoUpdated( std::shared_ptr< IResponseObject > );             //! sig on device info updated
    void sigOnNameUpdated( std::shared_ptr< IResponseObject > );             //! sig on device name updated
    void sigOnErrorUpdated( std::shared_ptr< IResponseObject > );            //! sig on error message changed/updated
    void sigOnGroupUpdated( std::shared_ptr< IResponseObject > );            //! sig on group updated (SoundTouch 10 only)

    private slots:
    void slotAuthenticationRequired( QNetworkReply *, QAuthenticator *authenticator );
    void slotOnHttpFinished( QNetworkReply *reply );
    void slotOnHttpReadyToRead( QNetworkReply *reply );
    void slotOnWSConnected( void );
    void slotOnWSDisConnected( void );
    void slotOnWSTextMessageReceived( QString message );
  };
}  // namespace bose_soundtoch_lib
#endif  // BSOUNDTOUCHDEVICE_HPP
