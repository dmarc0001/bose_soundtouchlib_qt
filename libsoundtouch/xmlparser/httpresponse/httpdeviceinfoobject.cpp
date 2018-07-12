#include "httpdeviceinfoobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpDeviceInfoObject::HttpDeviceInfoObject
   * @param xmlreader
   * @param parent
   */
  HttpDeviceInfoObject::HttpDeviceInfoObject( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "info" ) );
    resultType = ResultobjectType::R_DEVICE_INFO;
    //
    // Device ID finden (Attribute von <info>)
    //
    qDebug() << "...";
    deviceId = IResponseObject::getAttribute( domElem, QLatin1String( "deviceID" ) );
    qDebug() << "device id: " << deviceId;
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    QDomNodeList rootChildNodesList( domElem->childNodes() );
    for ( int nodeIdx = 0; nodeIdx < rootChildNodesList.length(); nodeIdx++ )
    {
      QDomNode currNode( rootChildNodesList.item( nodeIdx ) );
      if ( currNode.isNull() )
        continue;
      //
      // unterscheide die Knoten
      // der Name ist hier als QString
      //
      QString currName( currNode.nodeName() );
      // STRINGS
      if ( currName == QLatin1String( "name" ) )
      {
        //
        // Geräetename direkt als Element
        //
        deviceName = currNode.toElement().text();
        qDebug() << "device name: " << deviceName;
      }
      else if ( currName == QLatin1String( "type" ) )
      {
        //
        // Type des Gerätes als Element
        //
        deviceType = currNode.toElement().text();
        qDebug() << "device type: " << deviceType;
      }
      // unsupported strings 2018-05-01
      else if ( currName == QLatin1String( "margeAccountUUID" ) )
      {
        //
        // margeAccountUUID des Gerätes als Element
        //
        _margeAccountUUID = currNode.toElement().text();
        qDebug() << "module margeAccountUUID: " << _margeAccountUUID;
      }
      else if ( currName == QLatin1String( "margeURL" ) )
      {
        //
        // margeURL des Gerätes als Element
        //
        _margeURL = currNode.toElement().text();
        qDebug() << "module margeURL: " << _margeURL;
      }
      else if ( currName == QLatin1String( "moduleType" ) )
      {
        //
        // Modultype des Gerätes als Element
        //
        _moduleType = currNode.toElement().text();
        qDebug() << "module type: " << _moduleType;
      }
      else if ( currName == QLatin1String( "variant" ) )
      {
        //
        // Variante des Gerätes als Element
        //
        _variant = currNode.toElement().text();
        qDebug() << "module variant: " << _variant;
      }
      else if ( currName == QLatin1String( "variantMode" ) )
      {
        //
        // Variant Mode des Gerätes als Element
        //
        _variantMode = currNode.toElement().text();
        qDebug() << "module variant mode: " << _variantMode;
      }
      else if ( currName == QLatin1String( "countryCode" ) )
      {
        //
        // Ländercode des Gerätes als Element
        //
        _countryCode = currNode.toElement().text();
        qDebug() << "module country code: " << _countryCode;
      }
      else if ( currName == QLatin1String( "regionCode" ) )
      {
        //
        // Regioncode des Gerätes als Element
        //
        _regionCode = currNode.toElement().text();
        qDebug() << "module region code: " << _regionCode;
      }
      // OBJEKTE
      else if ( currName == QLatin1String( "components" ) )
      {
        //
        // Liste der Gerätekomponenten unterobjekte sind "component"
        //
        parseComponents( &currNode );
      }
      else if ( currName == QLatin1String( "networkInfo" ) )
      {
        //
        // Objekt mit Netzwerkinformationen
        //
        parseNetworkInfo( &currNode );
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag: " << currName << " --> " << currNode.toElement().text();
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
  void HttpDeviceInfoObject::parseComponents( QDomNode *node )
  {
    Q_ASSERT( node->nodeName() == QLatin1String( "components" ) );
    qDebug() << "...";
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    QDomNodeList childNodesList( node->childNodes() );
    for ( int nodeIdx = 0; nodeIdx < childNodesList.length(); nodeIdx++ )
    {
      QDomNode currNode( childNodesList.item( nodeIdx ) );
      if ( currNode.isNull() )
        continue;
      if ( currNode.nodeName() == QLatin1String( "component" ) )
      {
        //
        // einzelne Komponente parsen und in die Liste eintragen
        //
        parseSingleComponent( &currNode );
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag: " << currNode.nodeName() << " --> " << currNode.toElement().text();
      }
    }
  }

  void HttpDeviceInfoObject::parseSingleComponent( QDomNode *node )
  {
    Q_ASSERT( node->nodeName() == QLatin1String( "component" ) );
    qDebug() << "...";
    //
    // lese die Komponente aus und trage sie in die Liste der Komponenten ein
    //
    QDomNodeList childNodesList( node->childNodes() );
    for ( int nodeIdx = 0; nodeIdx < childNodesList.length(); nodeIdx++ )
    {
      QDomNode currNode( childNodesList.item( nodeIdx ) );
      if ( currNode.isNull() )
        continue;
      // neues lokales Element
      DeviceComponent currComponent;
      //
      // unterscheide die Knoten
      // der Name ist hier als QString
      //
      QString currName( currNode.nodeName() );
      //
      if ( currName == QLatin1String( "componentCategory" ) )
      {
        // UNSUPPORTED
        currComponent._componentCategory = currNode.toElement().text();
        qDebug() << "component category: " << currComponent._componentCategory;
      }
      else if ( currName == QLatin1String( "softwareVersion" ) )
      {
        currComponent.softwareVersion = currNode.toElement().text();
        qDebug() << "software version: " << currComponent.softwareVersion;
      }
      else if ( currName == QLatin1String( "serialNumber" ) )
      {
        currComponent.serialNumber = currNode.toElement().text();
        qDebug() << "serial number: " << currComponent.serialNumber;
      }
      else
      {
        qWarning() << "unsupported tag: " << currName << " --> " << currNode.toElement().text();
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
  void HttpDeviceInfoObject::parseNetworkInfo( QDomNode *node )
  {
    Q_ASSERT( node->nodeName() == QLatin1String( "networkInfo" ) );
    DeviceNetworkInfo singleDeviceNetworkInfo;
    qDebug() << "...";
    //
    // Network type finden (Attribute von <networkInfo>)
    //
    singleDeviceNetworkInfo._type = IResponseObject::getAttribute( node, QLatin1String( "type" ) );
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    QDomNodeList childNodesList( node->childNodes() );
    for ( int nodeIdx = 0; nodeIdx < childNodesList.length(); nodeIdx++ )
    {
      QDomNode currNode( childNodesList.item( nodeIdx ) );
      if ( currNode.isNull() )
        continue;
      //
      // unterscheide die Knoten
      // der Name ist hier als QString
      //
      QString currName( currNode.nodeName() );
      //
      if ( currName == QLatin1String( "macAddress" ) )
      {
        singleDeviceNetworkInfo.macAddress = currNode.toElement().text();
        qDebug() << "macAddress: " << singleDeviceNetworkInfo.macAddress;
      }
      else if ( currName == QLatin1String( "ipAddress" ) )
      {
        singleDeviceNetworkInfo.macAddress = currNode.toElement().text();
        qDebug() << "ipAddress: " << singleDeviceNetworkInfo.macAddress;
      }
      else if ( currName == QLatin1String( "type" ) )
      {
        // unsupported
        singleDeviceNetworkInfo._type = currNode.toElement().text();
        qDebug() << "type: " << singleDeviceNetworkInfo._type;
      }
      else
      {
        qWarning() << "unsupportet etag: " << currName << " --> " << currNode.toElement().text();
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
