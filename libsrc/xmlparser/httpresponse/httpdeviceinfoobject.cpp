#include "httpresponse/httpdeviceinfoobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpDeviceInfoObject::HttpDeviceInfoObject
   * @param xmlreader
   * @param parent
   */
  HttpDeviceInfoObject::HttpDeviceInfoObject( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "info" ) );
    resultType = ResultobjectType::R_DEVICE_INFO;
    //
    // Device ID finden (Attribute von <info>)
    //
    qDebug() << "...";
    deviceId = IResponseObject::getAttribute( reader, QLatin1String( "deviceID" ) );
    qDebug() << "device id: " << deviceId;
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      //
      // das nächste element bearbeiten, welches ist es?
      //

      // STRINGS
      if ( reader->name() == QLatin1String( "name" ) )
      {
        //
        // Geräetename direkt als Element
        //
        deviceName = reader->readElementText();
        qDebug() << "device name: " << deviceName;
      }
      else if ( reader->name() == QLatin1String( "type" ) )
      {
        //
        // Type des Gerätes als Element
        //
        deviceType = reader->readElementText();
        qDebug() << "device type: " << deviceType;
      }
      // unsupported strings 2018-05-01
      else if ( reader->name() == QLatin1String( "margeAccountUUID" ) )
      {
        //
        // margeAccountUUID des Gerätes als Element
        //
        _margeAccountUUID = reader->readElementText();
        qDebug() << "module margeAccountUUID: " << _margeAccountUUID;
      }
      else if ( reader->name() == QLatin1String( "margeURL" ) )
      {
        //
        // margeURL des Gerätes als Element
        //
        _margeURL = reader->readElementText();
        qDebug() << "module margeURL: " << _margeURL;
      }
      else if ( reader->name() == QLatin1String( "moduleType" ) )
      {
        //
        // Modultype des Gerätes als Element
        //
        _moduleType = reader->readElementText();
        qDebug() << "module type: " << _moduleType;
      }
      else if ( reader->name() == QLatin1String( "variant" ) )
      {
        //
        // Variante des Gerätes als Element
        //
        _variant = reader->readElementText();
        qDebug() << "module variant: " << _variant;
      }
      else if ( reader->name() == QLatin1String( "variantMode" ) )
      {
        //
        // Variant Mode des Gerätes als Element
        //
        _variantMode = reader->readElementText();
        qDebug() << "module variant mode: " << _variantMode;
      }
      else if ( reader->name() == QLatin1String( "countryCode" ) )
      {
        //
        // Ländercode des Gerätes als Element
        //
        _countryCode = reader->readElementText();
        qDebug() << "module country code: " << _countryCode;
      }
      else if ( reader->name() == QLatin1String( "regionCode" ) )
      {
        //
        // Regioncode des Gerätes als Element
        //
        _regionCode = reader->readElementText();
        qDebug() << "module region code: " << _regionCode;
      }
      // OBJEKTE
      else if ( reader->name() == QLatin1String( "components" ) )
      {
        //
        // Liste der Gerätekomponenten unterobjekte sind "component"
        //
        parseComponents();
      }
      else if ( reader->name() == QLatin1String( "networkInfo" ) )
      {
        //
        // Objekt mit Netzwerkinformationen
        //
        parseNetworkInfo();
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag: " << reader->name().toString() << " --> " << reader->readElementText();
      }
    }
  }

  /**
   * @brief HttpDeviceInfoObject::~HttpDeviceInfoObject
   */
  HttpDeviceInfoObject::~HttpDeviceInfoObject()
  {
    qDebug() << "...";
  }

  /**
   * @brief HttpDeviceInfoObject::parseComponents
   */
  void HttpDeviceInfoObject::parseComponents( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "components" ) );
    qDebug() << "...";
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      if ( reader->name() == QLatin1String( "component" ) )
      {
        //
        // einzelne Komponente parsen und in die Liste eintragen
        //
        parseSingleComponent();
      }
    }
  }

  void HttpDeviceInfoObject::parseSingleComponent( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "component" ) );
    qDebug() << "...";
    //
    // lese die Komponente aus und trage sie in die Liste der Komponenten ein
    //
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      // neues lokales Element
      DeviceComponent currComponent;
      //
      // welchen Eintrag hab ich gefunden?
      //
      if ( reader->name() == QLatin1String( "componentCategory" ) )
      {
        // UNSUPPORTED
        currComponent._componentCategory = reader->readElementText();
        qDebug() << "component category: " << currComponent._componentCategory;
      }
      else if ( reader->name() == QLatin1String( "softwareVersion" ) )
      {
        currComponent.softwareVersion = reader->readElementText();
        qDebug() << "software version: " << currComponent.softwareVersion;
      }
      else if ( reader->name() == QLatin1String( "serialNumber" ) )
      {
        currComponent.serialNumber = reader->readElementText();
        qDebug() << "serial number: " << currComponent.serialNumber;
      }
      else
      {
        qWarning() << "unsupported tag: " << reader->name().toString() << " --> " << reader->readElementText();
      }
      //
      // und die Komponente hinzufügen
      // kopierkonstruktor...
      //
      deviceComponents.append( currComponent );
    }
  }

  /**
   * @brief HttpDeviceInfoObject::parseNetworkInfo
   */
  void HttpDeviceInfoObject::parseNetworkInfo( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "networkInfo" ) );
    DeviceNetworkInfo singleDeviceNetworkInfo;
    qDebug() << "...";
    //
    // Network type finden (Attribute von <networkInfo>)
    //
    singleDeviceNetworkInfo._type = IResponseObject::getAttribute( reader, QLatin1String( "type" ) );
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    //
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      if ( reader->name() == QLatin1String( "macAddress" ) )
      {
        singleDeviceNetworkInfo.macAddress = reader->readElementText();
        qDebug() << "macAddress: " << singleDeviceNetworkInfo.macAddress;
      }
      else if ( reader->name() == QLatin1String( "ipAddress" ) )
      {
        singleDeviceNetworkInfo.macAddress = reader->readElementText();
        qDebug() << "ipAddress: " << singleDeviceNetworkInfo.macAddress;
      }
      else if ( reader->name() == QLatin1String( "type" ) )
      {
        // unsupported
        singleDeviceNetworkInfo._type = reader->readElementText();
        qDebug() << "type: " << singleDeviceNetworkInfo._type;
      }
      else
      {
        qWarning() << "unsupportet etag: " << reader->name().toString() << " --> " << reader->readElementText();
      }
    }
    deviceNetworkInfos.append( singleDeviceNetworkInfo );
  }

  //
  // GETTER
  //

  QString HttpDeviceInfoObject::getDeviceName() const
  {
    return deviceName;
  }

  QString HttpDeviceInfoObject::getDeviceType() const
  {
    return deviceType;
  }

  QString HttpDeviceInfoObject::getMargeAccountUUID() const
  {
    return _margeAccountUUID;
  }

  QString HttpDeviceInfoObject::getMargeURL() const
  {
    return _margeURL;
  }

  QString HttpDeviceInfoObject::getModuleType() const
  {
    return _moduleType;
  }

  QString HttpDeviceInfoObject::getVariant() const
  {
    return _variant;
  }

  QString HttpDeviceInfoObject::getVariantMode() const
  {
    return _variantMode;
  }

  QString HttpDeviceInfoObject::getCountryCode() const
  {
    return _countryCode;
  }

  QString HttpDeviceInfoObject::getRegionCode() const
  {
    return _regionCode;
  }

  QVector< DeviceComponent > HttpDeviceInfoObject::getDeviceComponents() const
  {
    return deviceComponents;
  }

  QVector< DeviceNetworkInfo > HttpDeviceInfoObject::getDeviceNetworkInfos() const
  {
    return deviceNetworkInfos;
  }

}  // namespace bose_soundtoch_lib
