#ifndef BSOUNDTOUCH_GLOBAL_HPP
#define BSOUNDTOUCH_GLOBAL_HPP

#include <qglobal.h>
#include <QObject>
#include <QPair>
#include <QString>
#include <QVector>
#include <QXmlStreamReader>
#include <QtDebug>
#include <memory>

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

  class ContentItem;

  class DeviceRecent
  {
    public:
    QString deviceId;
    qint64 utcTime;
    qint64 id;
    std::shared_ptr< ContentItem > contentItem;
  };

  class ContentItem : public QObject
  {
    Q_OBJECT
    protected:
    QXmlStreamReader *reader;

    public:
    QString source;
    QString location;
    QString sourceAccount;
    bool isPresetable;
    QString itemName;
    QString containerArt;
    explicit ContentItem( QXmlStreamReader *xmlreader, QObject *parent );
    ContentItem( const ContentItem &cp ) = default;
    // ContentItem( const ContentItem *cp );
    ~ContentItem();

    private:
    QString getAttibute( QXmlStreamReader *reader, QLatin1String name ) const;
  };

  class PlayingArt
  {
    public:
    QString artImageStatus;
    QString artUrl;
  };

  class PlayingTime
  {
    public:
    int total_sec;
    int current_sec;
  };

  class DeviceConnectionStatusInfo
  {
    public:
    QString status;
    QString deviceName;
  };

  class DeviceComponent
  {
    public:
    QString _componentCategory;
    QString softwareVersion;
    QString serialNumber;
  };

  class DeviceNetworkInfo
  {
    public:
    QString _type;  //! unsupported
    QString macAddress;
    QString ipAddress;
  };

  class DevicePreset
  {
    public:
    QString id;
    quint64 createdOn;
    quint64 updatedOn;
    std::shared_ptr< ContentItem > contentItem;
    DevicePreset( void ){};
    DevicePreset( const DevicePreset &cp ) = default;
  };

  class SourceItem
  {
    public:
    QString source;
    QString sourceAccount;
    QString status;
    bool isLocal = false;
    bool multiroomallowed = false;
    QString Content;
  };

  class GroupRole
  {
    public:
    QString deviceId;
    QString role;  // left or right
    QString ipAddress;
  };

  class ResponseError
  {
    public:
    int value;
    QString name;
    QString severity;
    QString text;
  };

}  // namespace bose_soundtoch_lib
#endif  // BSOUNDTOUCH_GLOBAL_HPP
