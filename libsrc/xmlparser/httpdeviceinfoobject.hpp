#ifndef DEVICEINFOOBJECT_HPP
#define DEVICEINFOOBJECT_HPP

#include <qglobal.h>
#include <QXmlStreamReader>
#include "../logging/Logger.hpp"
#include "iresponseobject.hpp"
#include "soundtouch_response.hpp"

namespace radio
{
  class HttpDeviceInfoObject : public IResponseObject
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
    HttpDeviceInfoObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent = nullptr );
    ~HttpDeviceInfoObject() override;
    // Getter Funktionen
    QString getDeviceId() const;
    QString getDeviceName() const;
    QString getDeviceType() const;
    QString getMargeAccountUUID() const;
    QString getMargeURL() const;
    QString getModuleType() const;
    QString getVariant() const;
    QString getVariantMode() const;
    QString getCountryCode() const;
    QString getRegionCode() const;
    QVector< DeviceComponent > getDeviceComponents() const;
    QVector< DeviceNetworkInfo > getDeviceNetworkInfos() const;

    private:
    void parseComponents( void );
    void parseSingleComponent( void );
    void parseNetworkInfo( void );
  };
}  // namespace radio
#endif  // DEVICEINFOOBJECT_HPP
