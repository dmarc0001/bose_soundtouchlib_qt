#ifndef BSOUNDTOUCH_GLOBAL_HPP
#define BSOUNDTOUCH_GLOBAL_HPP

#include <qglobal.h>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QObject>
#include <QPair>
#include <QString>
#include <QVector>
#include <QtDebug>
#include <memory>
#include "../libsoundtouch_global.hpp"

/*
#ifdef SOUNDTOUCH_QT_LIB_IMPORT
#define SOUNDTOUCH_QT_LIBSHARED_EXPORT Q_DECL_IMPORT
#else
#ifdef SOUNDTOUCH_QT_LIB_EXPORT
#define SOUNDTOUCH_QT_LIBSHARED_EXPORT Q_DECL_EXPORT
#else
#define SOUNDTOUCH_QT_LIBSHARED_EXPORT
#endif
#endif
*/

namespace bose_soundtoch_lib
{
  //
  // Definitionen um die Benennung der Parameter lesbarer zu machen
  // Parameter "IPADDR", "MACADDR/DEVICEID"
  //
  //! Paare Soundtouch <"IPADDR", "MACADDR/DEVICEID">
  using SoundTouchMemberObject = QPair< QString, QString >;
  //! Liste mit Paaren
  using SoundTouchMemberList = QVector< SoundTouchMemberObject >;

  //! typ des Resultobjektes
  enum class ResultobjectType : quint8
  {
    R_OK,
    R_ERROR,
    R_DEVICE_INFO,
    R_NOW_PLAYING,
    R_VOLUME,
    R_SOURCES,
    R_ZONES,
    R_BASS_CAPABILITIES,
    R_BASS,
    R_PRESETS,
    R_GROUPS,
    U_SDKINFO,
    U_PRESETS,
    U_NOWPLAYING,
    U_SELECTION,
    U_VOLUME,
    U_BASS,
    U_ZONE,
    U_INFO,
    U_NAME,
    U_ERROR,
    U_GROUP,
    U_BROWSE_UNSUPPORTED,
    U_RECENTS_UNSUPPORTED,
    U_SOURCES_UNSUPPORTED,
    U_LANGUAGE_UNSUPPORTED,
    U_USER_ACTIVITY_UPDATED_UNSUPPORTED,
    U_USER_INACTIVITY_UPDATED_UNSUPPORTED,
    U_CONNECTION_STATE_UPDATED_UNSUPPORTED,
    U_AUDIOPRODUCT_TONECONTROLS_UNSUPPORTED,
    U_AUDIOPRODUCT_LEVELCONTROLS_UNSUPPORTED,
    U_AUDIO_SP_CONTROLS_UNSUPPORTED,
    R_UNKNOWN
  };

  class SOUNDTOUCH_QT_LIBSHARED_EXPORT ContentItem;

  //! Klasse speichert zurückliegende gewählte Sourcen des Gerätes
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT DeviceRecent
  {
    public:
    QString deviceId;  //! welches Gerät
    qint64 utcTime;    //! zu welcher zeit
    qint64 id;
    std::shared_ptr< ContentItem > contentItem;  //! genaue Beschreibung der Source
  };

  //! Genaue Beschreibung einer Medienquelle
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT ContentItem : public QObject
  {
    Q_OBJECT
    protected:
    QDomNode *node;

    public:
    QString source;
    QString type;
    QString location;
    QString sourceAccount;
    bool isPresetable;
    QString itemName;
    QString containerArt;
    explicit ContentItem( QDomNode *node, QObject *parent );
    ContentItem( const ContentItem &cp ) = default;
    // ContentItem( const ContentItem *cp );
    ~ContentItem();
  };

  //! Beschreibung des Logos der Quelle
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT PlayingArt
  {
    public:
    QString artImageStatus;
    QString artUrl;
  };

  //! Soeilzeit der Quelle (total/aktuell)
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT PlayingTime
  {
    public:
    int total_sec;
    int current_sec;
  };

  //! Verbindungsstatus des Gerätes
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT DeviceConnectionStatusInfo
  {
    public:
    QString status;
    QString deviceName;
  };

  //! Teil der Geräteinfo, beschreibt Teile der Harware
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT DeviceComponent
  {
    public:
    QString _componentCategory;
    QString softwareVersion;
    QString serialNumber;
  };

  //! Netzwerk des Gerätes (auch mnehrfach, z.B WLAN/Ethernet)
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT DeviceNetworkInfo
  {
    public:
    QString _type;  //! unsupported
    QString macAddress;
    QString ipAddress;
  };

  //! Die Belegung der sechs Stationstasten/Presets
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT DevicePreset
  {
    public:
    QString id;
    quint64 createdOn;
    quint64 updatedOn;
    std::shared_ptr< ContentItem > contentItem;
    DevicePreset( void ){};
    DevicePreset( const DevicePreset & ) = default;
  };

  //! Beschreibung einer Quelle (Intennet, BLuethooth etc)
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT SourceItem
  {
    public:
    QString source;
    QString sourceAccount;
    QString status;
    bool isLocal = false;
    bool multiroomallowed = false;
    QString Content;
  };

  //! SoundTouch 10 - Rolle eines Gerätes in der Gruppe
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT GroupRole
  {
    public:
    QString deviceId;
    QString role;  // left or right
    QString ipAddress;
  };

  //! Inhalt einer Fehlermeldung bei http response
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT ResponseError
  {
    public:
    int value;
    QString name;
    QString severity;
    QString text;
  };

}  // namespace bose_soundtoch_lib
#endif  // BSOUNDTOUCH_GLOBAL_HPP
