#ifndef DEVICEINFOOBJECT_HPP
#define DEVICEINFOOBJECT_HPP

#include <qglobal.h>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QtDebug>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class HttpDeviceInfoObject : public IResponseObject
  {
    private:
    QString deviceName;         //! Gerätename
    QString deviceType;         //! gerätetyp (von Bose Soundtouch)
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
    HttpDeviceInfoObject( QDomElement *domElem, QObject *parent = nullptr );
    ~HttpDeviceInfoObject() override;
    //
    // GETTER
    //
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
    void parseComponents( QDomNode *node );       //! Kmponentenliste parsen
    void parseSingleComponent( QDomNode *node );  //! Einzelne Kompnente parsen
    void parseNetworkInfo( QDomNode *node );      //! Network info lesen
  };
}  // namespace bose_soundtoch_lib
#endif  // DEVICEINFOOBJECT_HPP
