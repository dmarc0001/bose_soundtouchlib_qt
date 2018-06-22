#ifndef BSOUNDTOUCH_GLOBAL_HPP
#define BSOUNDTOUCH_GLOBAL_HPP

#include <QPair>
#include <QString>
#include <QVector>

namespace radio
{
  //
  // Definitionen um die Benennung der Parameter lesbarer zu machen
  // Parameter "IPADDR", "MACADDR/DEVICEID"
  //
  //! Paare Soundtouch <"IPADDR", "MACADDR/DEVICEID">
  using SoundTouchMemberObject = QPair< QString, QString >;
  //! Liste mit Paaren
  using SoundTouchMemberList = QVector< SoundTouchMemberObject >;

  class ContentItem
  {
    public:
    QString source;
    QString location;
    QString sourceAccount;
    bool isPresetable;
    QString itemName;
    QString containerArt;
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
    ContentItem contentItem;
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

}  // namespace radio
#endif  // BSOUNDTOUCH_GLOBAL_HPP
