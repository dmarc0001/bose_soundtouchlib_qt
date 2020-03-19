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
#include "libsoundtouch_global.hpp"
#include "websocket/bwebsocket.hpp"
#include "xmlparser/bsoundtouch_global.hpp"
#include "xmlparser/iresponseobject.hpp"

// bose developer dmarc0001
// pw dd03353e83

namespace bose_soundtoch_lib
{
  using IResponseObjPtr = std::shared_ptr< IResponseObject >;

  class SOUNDTOUCH_QT_LIBSHARED_EXPORT BSoundTouchDevice : public QObject
  {
    Q_OBJECT

    public:
    //! interne Klasse möglichkeiten der keys
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
    //! Aufzählung der Playstsati
    enum class bose_playstate : int
    {
      PLAY_STATE,
      PAUSE_STATE,
      STOP_STATE,
      BUFFERING_STATE,
      UNKNOWN_STATE
    };

    private:
    QString hostname;
    quint16 wsPort;
    quint16 httpPort;
    QString id;
    QString ip;
    std::unique_ptr< BWebsocket > webSocket;
    QtMsgType threshold;
    QUrl url;
    QNetworkAccessManager qnam;                              //! Access Manager für network
    static const QMap< bose_keystate, QString > keystati;    //! 1 mögliche Keystati
    static const QMap< bose_key, QString > keynames;         //! Namen der Tasten
    static const QMap< bose_playstate, QString > playstate;  //! mögliche Playstati
    static const char *subproto;                             //! Subprotokoll
    constexpr static int timeoutMillis = 80;                 //! Timout in Milisekunden
    static const QString version;                            //! Versionsnummer

    public:
    explicit BSoundTouchDevice( QString &stHost,
                                quint16 stWSPort,
                                quint16 stHttpPort,
                                QObject *parent = nullptr,
                                QtMsgType sth = QtMsgType::QtFatalMsg );
    ~BSoundTouchDevice();
    void setHostname( const QString &stHost );
    void setWSPort( quint16 stWSPort );
    void setHttpPort( quint16 stHttpPort );
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
    bose_key getKeyForName( const QString &name );
    QString getKeyStateName( bose_keystate state ) const;
    QString getPlayStateName( bose_playstate state ) const;
    bose_playstate getKeyPlayStateName( const QString playstate );
    void setKey( bose_key whichkey, bose_keystate keystate, QString sender = "Gabbo" );       // POST
    void selectSource( const QString &source, const QString &account );                       //! AUX/AMAZON/INTERNET etc...
    void setBass( int bass );                                                                 // POST
    void setVolume( int volume );                                                             // POST
    void setZone( const QString &masterId, const SoundTouchMemberList &memberList );          //! create zone
    void addZoneSlave( const QString &masterId, const SoundTouchMemberList &memberList );     // POST
    void removeZoneSlave( const QString &masterId, const SoundTouchMemberList &memberList );  // POST
    void setDeviceName( QString &name );                                                      // POST
    QString getId() const;                                                                    //! ID des Gerätes (optional)
    void setId( const QString &value );                                                       //! ID des Gerätes (optional)
    QString getIpString() const;                                                              //! IP des Gerätes (optional)
    void setIpString( const QString &value );                                                 //! IP des Gerätes (optional)
    // websocket überwachung
    void addVolumeListener( void );
    QString getVersion() const;
    void connectWs( void );     //! verbinde gerät mit Websocket, falls noch nicht geschehen
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
    void sigAuthenticationRequired( QNetworkReply *repl, QAuthenticator *authenticator );  //! sig wenn Authentifizierung gefordert
    void sigOnSourcesResult( IResponseObjPtr );           //! wenn Sourcen als Antwort empfangen wurden
    void sigOnBassCapabilitiesResult( IResponseObjPtr );  //! wenn Bass Möglickkeiten als Antwort empfangen wurde
    void sigOnZonesResult( IResponseObjPtr );             //! wenn Zohnen als Antwort empfangen wurden
    void sigOnVolumeResult( IResponseObjPtr );            //! wenn volume als Antwort empfangen wurden
    void sigOnPresetsResult( IResponseObjPtr );           //! wenn presets als Antwort empfangen wurden
    void sigOnBassResult( IResponseObjPtr );              //! wenn Bass als Antwort empfangen wurden
    void sigOnDeviceInfoResult( IResponseObjPtr );        //! wenn device Info als Antwort empfangen wurden
    void sigOnNowPlayingResult( IResponseObjPtr );        //! wenn now playing empfangen wurde (ist genau wie update)
    void sigOnOkResult( IResponseObjPtr );                //! wenn OK empfangen
    void sigOnErrorResult( IResponseObjPtr );             //! Fehler bei der Abfrage
    void sigOnGroupResult( IResponseObjPtr );             //! Gruppe erfragt
    void sigOnWSConnected( void );                        //! sig wenn Websocket erbunden ist
    void sigOnWSDisConnected( void );                     //! sif wenn Websocket getrennt wurde
    void sigOnWSTextMessageReceived( QString message );   //! sig wenn Websocket etwas gesendet hat
    void sigOnRequestAnswer( IResponseObjPtr response );  //! Antwort auf ein HTTP Request
    void sigOnPresetsUpdated( IResponseObjPtr );          //! sig on preset Update
    void sigOnNowPlayingUpdated( IResponseObjPtr );       //! sig on now playing Update
    void sigOnPresetSelectionUpdated( IResponseObjPtr );  //! sig on selction an Update
    void sigOnVolumeUpdated( IResponseObjPtr );           //! sig on volume changed/updated
    void sigOnBassUpdated( IResponseObjPtr );             //! sig on bass property updated
    void sigOnZoneUpdated( IResponseObjPtr );             //! sig on zone updated
    void sigOnInfoUpdated( IResponseObjPtr );             //! sig on device info updated
    void sigOnNameUpdated( IResponseObjPtr );             //! sig on device name updated
    void sigOnGroupUpdated( IResponseObjPtr );            //! sig on group updated SoundTouch 10 only
    void sigOnErrorUpdated( IResponseObjPtr );            //! sig on error message changed or updated

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
