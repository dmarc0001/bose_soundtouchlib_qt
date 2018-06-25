#ifndef BSOUNDTOUCHDEVICE_HPP
#define BSOUNDTOUCHDEVICE_HPP

#include <QAuthenticator>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QPair>
#include <QVector>
#include <memory>

#include "logging/Logger.hpp"
#include "websocket/bwebsocket.hpp"
#include "xmlparser/soundtouch_response.hpp"

#if defined( SOUNDTOUCH_QT_LIB_LIBRARY )
#define SOUNDTOUCH_QT_LIBSHARED_EXPORT Q_DECL_EXPORT
#else
#define SOUNDTOUCH_QT_LIBSHARED_EXPORT Q_DECL_IMPORT
#endif

// bose developer dmarc0001
// pw dd03353e83

namespace radio
{
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT BSoundTouchDevice : public QObject
  {
    Q_OBJECT

    public:
    enum class bose_keystate : int
    {
      KEY_PRESSED,
      KEY_RELEASED,
      KEY_TOGGLE
    };
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
    static const char *keynames[];

    private:
    QString hostname;
    qint16 wsPort;
    qint16 httpPort;
    std::shared_ptr< Logger > lg;
    // bool httpRequestAborted;
    // volatile bool httpRequestIsWorking;
    // volatile qint16 httpTimeout;
    QUrl url;
    QNetworkAccessManager qnam;
    // QNetworkReply *reply;
    static const char *keystati[];
    // static const char *keynames[];
    static const char *subproto;
    constexpr static int timeoutMillis = 80;
    std::unique_ptr< BWebsocket > webSocket;

    public:
    explicit BSoundTouchDevice( QString &stHost,
                                qint16 stWSPort,
                                qint16 stHttpPort,
                                std::shared_ptr< Logger > logger,
                                QObject *parent = nullptr );
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
    void getGroup( void );                                                                    //! Soundtouch 10 only
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

    signals:
    void sigAuthenticationRequired( QNetworkReply *repl, QAuthenticator *authenticator );

    private slots:
    void slotAuthenticationRequired( QNetworkReply *, QAuthenticator *authenticator );
    void slotOnHttpFinished( QNetworkReply *reply );
    void slotOnHttpReadyToRead( QNetworkReply *reply );
  };
}  // namespace radio
#endif  // BSOUNDTOUCHDEVICE_HPP