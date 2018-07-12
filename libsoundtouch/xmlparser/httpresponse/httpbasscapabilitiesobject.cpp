#include "httpbasscapabilitiesobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpBassCapabilitiesObject::HttpBassCapabilitiesObject Objekt parsen
   * @param xmlreader
   * @param parent
   */
  HttpBassCapabilitiesObject::HttpBassCapabilitiesObject( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "bassCapabilities" ) );
    resultType = ResultobjectType::R_BASS_CAPABILITIES;
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
      QString currName( currNode.nodeName() );
      //
      if ( currName == QLatin1String( "bassAvailable" ) )
      {
        if ( currNode.toElement().text() == QLatin1String( "true" ) )
        {
          bassAvailable = true;
          qDebug() << "bass availible: " << bassAvailable;
        }
      }
      else if ( currName == QLatin1String( "bassMin" ) )
      {
        bassMin = currNode.toElement().text().toInt();
        qDebug() << "bass minimum value: " << bassMin;
      }
      else if ( currName == QLatin1String( "bassMax" ) )
      {
        bassMax = currNode.toElement().text().toInt();
        qDebug() << "bass maximum value: " << bassMax;
      }
      else if ( currName == QLatin1String( "bassDefault" ) )
      {
        bassDefault = currNode.toElement().text().toInt();
        qDebug() << "bass default value: " << bassDefault;
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
   * @brief HttpBassCapabilitiesObject::~HttpBassCapabilitiesObject
   */
  HttpBassCapabilitiesObject::~HttpBassCapabilitiesObject()
  {
    qDebug() << "...";
  }

  //
  // GETTER Methoden
  //

  bool HttpBassCapabilitiesObject::getBassAvailable() const
  {
    return bassAvailable;
  }

  int HttpBassCapabilitiesObject::getBassMin() const
  {
    return bassMin;
  }

  int HttpBassCapabilitiesObject::getBassMax() const
  {
    return bassMax;
  }

  int HttpBassCapabilitiesObject::getBassDefault() const
  {
    return bassDefault;
  }

}  // namespace bose_soundtoch_lib
