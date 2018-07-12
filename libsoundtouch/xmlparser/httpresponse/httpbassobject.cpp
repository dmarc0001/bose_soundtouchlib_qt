#include "httpbassobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpBassObject::HttpBassObject
   * @param xmlreader
   * @param parent
   */
  HttpBassObject::HttpBassObject( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "bass" ) );
    resultType = ResultobjectType::R_BASS;
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
      //
      if ( currName == QLatin1String( "targetbass" ) )
      {
        //
        // Bass zieleinstellung
        //
        targetbass = currNode.toElement().text().toInt();
        qDebug() << "bass target is " << targetbass;
      }
      else if ( currName == QLatin1String( "actualbass" ) )
      {
        //
        // Bass aktuelle einstellung
        //
        actualbass = currNode.toElement().text().toInt();
        qDebug() << "bass is actual " << actualbass;
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

  HttpBassObject::~HttpBassObject()
  {
    qDebug() << "...";
  }

  //
  // GETTER
  //

  int HttpBassObject::getTargetBass() const
  {
    return targetbass;
  }

  int HttpBassObject::getActualBass() const
  {
    return actualbass;
  }

}  // namespace bose_soundtoch_lib
