#include "deviceinfoobject.hpp"

namespace radio
{
  DeviceInfoObject::DeviceInfoObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "info" ) );
    resultType = ResultobjectType::R_DEVICE_INFO;
    //
    // Device ID finden (Attribute von <info>)
    //
    lg->debug( "DeviceInfoObject::DeviceInfoObject..." );
    if ( reader->name() == QLatin1String( "info" ) )
    {
      lg->debug( "DeviceInfoObject::DeviceInfoObject: check for attribute \"deviceID\"..." );
      QXmlStreamAttributes attr = reader->attributes();
      if ( attr.hasAttribute( QLatin1String( "deviceID" ) ) )
      {
        deviceId = attr.value( QLatin1String( "deviceID" ) ).toString();
        lg->debug( QString( "DeviceInfoObject::DeviceInfoObject: attribute \"deviceID\" has value %1" ).arg( deviceId ) );
      }
      else
      {
        lg->warn( "DeviceInfoObject::DeviceInfoObject: there is no attribute \"deviceID\"..." );
      }
    }
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( reader->readNextStartElement() && !reader->hasError() )
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
        lg->debug( QString( "DeviceInfoObject::DeviceInfoObject: device name: " ).append( deviceName ) );
      }
      else if ( reader->name() == QLatin1String( "type" ) )
      {
        //
        // Type des Gerätes als Element
        //
        deviceType = reader->readElementText();
        lg->debug( QString( "DeviceInfoObject::DeviceInfoObject: device type: " ).append( deviceType ) );
      }
      // unsupported strings 2018-05-01
      else if ( reader->name() == QLatin1String( "margeAccountUUID" ) )
      {
        //
        // margeAccountUUID des Gerätes als Element
        //
        _margeAccountUUID = reader->readElementText();
        lg->debug( QString( "DeviceInfoObject::DeviceInfoObject: module margeAccountUUID: " ).append( _margeAccountUUID ) );
      }
      else if ( reader->name() == QLatin1String( "margeURL" ) )
      {
        //
        // margeURL des Gerätes als Element
        //
        _margeURL = reader->readElementText();
        lg->debug( QString( "DeviceInfoObject::DeviceInfoObject: module margeURL: " ).append( _margeURL ) );
      }
      else if ( reader->name() == QLatin1String( "moduleType" ) )
      {
        //
        // Modultype des Gerätes als Element
        //
        _moduleType = reader->readElementText();
        lg->debug( QString( "DeviceInfoObject::DeviceInfoObject: module type: " ).append( _moduleType ) );
      }
      else if ( reader->name() == QLatin1String( "variant" ) )
      {
        //
        // Variante des Gerätes als Element
        //
        _variant = reader->readElementText();
        lg->debug( QString( "DeviceInfoObject::DeviceInfoObject: module variant: " ).append( _variant ) );
      }
      else if ( reader->name() == QLatin1String( "variantMode" ) )
      {
        //
        // Variant Mode des Gerätes als Element
        //
        _variantMode = reader->readElementText();
        lg->debug( QString( "DeviceInfoObject::DeviceInfoObject: module variant mode: " ).append( _variantMode ) );
      }
      else if ( reader->name() == QLatin1String( "countryCode" ) )
      {
        //
        // Ländercode des Gerätes als Element
        //
        _countryCode = reader->readElementText();
        lg->debug( QString( "DeviceInfoObject::DeviceInfoObject: module country code: " ).append( _countryCode ) );
      }
      else if ( reader->name() == QLatin1String( "regionCode" ) )
      {
        //
        // Regioncode des Gerätes als Element
        //
        _regionCode = reader->readElementText();
        lg->debug( QString( "DeviceInfoObject::DeviceInfoObject: module region code: " ).append( _regionCode ) );
      }
      // OBJEKTE
      else if ( reader->name() == QLatin1String( "components" ) )
      {
        //
        // Liste der Gerätekomponenten unterobjekte sind "component"
        //
        lg->debug( "DeviceInfoObject::DeviceInfoObject: components detected." );
        parseComponents();
      }
      else if ( reader->name() == QLatin1String( "networkInfo" ) )
      {
        //
        // Objekt mit Netzwerkinformationen
        //
        lg->debug( "DeviceInfoObject::DeviceInfoObject: network info detected." );
        parseNetworkInfo();
      }
      else
      {
        //
        // unsupportet elements
        //
        lg->debug( QString( "DeviceInfoObject::DeviceInfoObject: %1 -> %2" )
                       .arg( reader->name().toString() )
                       .arg( reader->readElementText() ) );
      }
    }
  }

  DeviceInfoObject::~DeviceInfoObject()
  {
    lg->debug( "DeviceInfoObject::~DeviceInfoObject..." );
  }

  void DeviceInfoObject::parseComponents( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "components" ) );
    lg->debug( "DeviceInfoObject::parseComponents..." );

    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( reader->readNextStartElement() )
    {
      lg->debug( "DeviceInfoObject::parseComponents: single component detected." );
      if ( reader->name() == QLatin1String( "component" ) )
      {
        //
        // einzelne Komponente parsen und in die Liste eintragen
        //
        parseSingleComponent();
      }
    }
  }

  void DeviceInfoObject::parseSingleComponent( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "component" ) );

    //
    // lese die Komponente aus und trage sie in die Liste der Komponenten ein
    //
    lg->debug( "DeviceInfoObject::parseSingleComponent..." );
    while ( reader->readNextStartElement() )
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
        lg->debug(
            QString( "DeviceInfoObject::parseSingleComponent: component category: %1" ).arg( currComponent._componentCategory ) );
      }
      else if ( reader->name() == QLatin1String( "softwareVersion" ) )
      {
        currComponent.softwareVersion = reader->readElementText();
        lg->debug( QString( "DeviceInfoObject::parseSingleComponent: software version: %1" ).arg( currComponent.softwareVersion ) );
      }
      else if ( reader->name() == QLatin1String( "serialNumber" ) )
      {
        currComponent.serialNumber = reader->readElementText();
        lg->debug( QString( "DeviceInfoObject::parseSingleComponent: serial number: %1" ).arg( currComponent.serialNumber ) );
      }
      else
      {
        lg->warn( QString( "DeviceInfoObject::parseSingleComponent: unsupportet entry %1 -> %2 " )
                      .arg( reader->name().toString() )
                      .arg( reader->readElementText() ) );
      }
      //
      // und die Komponente hinzufügen
      // kopierkonstruktor...
      //
      deviceComponents.append( currComponent );
    }
    lg->debug( "DeviceInfoObject::parseSingleComponent: finished." );
  }

  void DeviceInfoObject::parseNetworkInfo( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "networkInfo" ) );
    DeviceNetworkInfo singleDeviceNetworkInfo;
    //
    // Network type finden (Attribute von <networkInfo>)
    //
    lg->debug( "DeviceInfoObject::parseNetworkInfo..." );
    if ( reader->name() == QLatin1String( "networkInfo" ) )
    {
      lg->debug( "DeviceInfoObject::parseNetworkInfo: check for attribute \"type\"..." );
      QXmlStreamAttributes attr = reader->attributes();
      if ( attr.hasAttribute( QLatin1String( "type" ) ) )
      {
        singleDeviceNetworkInfo._type = attr.value( QLatin1String( "type" ) ).toString();
        lg->debug( QString( "DeviceInfoObject::parseNetworkInfo: attribute \"type\" has value %1" ).arg( deviceId ) );
      }
      else
      {
        lg->warn( "DeviceInfoObject::DeviceInfoObject: there is no attribute \"type\"..." );
      }
    }
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    //
    while ( reader->readNextStartElement() )
    {
      if ( reader->name() == QLatin1String( "macAddress" ) )
      {
        singleDeviceNetworkInfo.macAddress = reader->readElementText();
        lg->debug( QString( "DeviceInfoObject::parseNetworkInfo: macAddress: %1" ).arg( singleDeviceNetworkInfo.macAddress ) );
      }
      else if ( reader->name() == QLatin1String( "ipAddress" ) )
      {
        singleDeviceNetworkInfo.macAddress = reader->readElementText();
        lg->debug( QString( "DeviceInfoObject::parseNetworkInfo: ipAddress: %1" ).arg( singleDeviceNetworkInfo.macAddress ) );
      }
      else if ( reader->name() == QLatin1String( "type" ) )
      {
        // unsupported
        singleDeviceNetworkInfo._type = reader->readElementText();
        lg->debug( QString( "DeviceInfoObject::parseNetworkInfo: type: %1" ).arg( singleDeviceNetworkInfo._type ) );
      }
      else
      {
        lg->warn( QString( "DeviceInfoObject::parseNetworkInfo: unsupportet entry %1 -> %2 " )
                      .arg( reader->name().toString() )
                      .arg( reader->readElementText() ) );
      }
    }
    deviceNetworkInfos.append( singleDeviceNetworkInfo );
  }
}  // namespace radio
