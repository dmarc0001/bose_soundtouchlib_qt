#ifndef DEVICEINFOOBJECT_HPP
#define DEVICEINFOOBJECT_HPP

#include <qglobal.h>
#include <QXmlStreamReader>
#include "../logging/Logger.hpp"
#include "iresponseobject.hpp"

namespace radio
{
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

  class DeviceInfoObject : public IResponseObject
  {
    private:
    QString deviceId;
    QString deviceName;
    QString deviceType;
    QString _margeAccountUUID;  //! unsupported
    QString _margeURL;          //! unsupported
    QString _moduleType;        //! unsupported
    QString _variant;           //! unsupported
    QString _variantMode;       //! unsupported
    QString _countryCode;       //! unsupported
    QString _regionCode;        //! unsupported
    QVector< DeviceComponent > deviceComponents;
    QVector< DeviceNetworkInfo > deviceNetworkInfos;

    public:
    DeviceInfoObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent = nullptr );

    private:
    void parseComponents( void );
    void parseSingleComponent( void );
    void parseNetworkInfo( void );
  };
}  // namespace radio
#endif  // DEVICEINFOOBJECT_HPP
