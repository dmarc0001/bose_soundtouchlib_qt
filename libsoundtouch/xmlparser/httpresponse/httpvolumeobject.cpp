#include "httpvolumeobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpVolumeObject::HttpVolumeObject
   * @param xmlreader
   * @param parent
   */
  HttpVolumeObject::HttpVolumeObject( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "volume" ) );
    resultType = ResultobjectType::R_VOLUME;
    //
    // Device ID finden (Attribute von <volume>)
    //
    qDebug() << "...";
    deviceId = IResponseObject::getAttribute( domElem, QLatin1String( "deviceID" ) );
    qDebug() << "device: " << deviceId;
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
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
      if ( currName == QLatin1String( "targetvolume" ) )
      {
        //
        // zu erreichende Lautstärke (0..100)
        //
        targetvolume = currNode.toElement().text().toInt();
        qDebug() << "target volume: " << targetvolume;
      }
      else if ( currName == QLatin1String( "actualvolume" ) )
      {
        //
        // aktuelle Lautstärke (0..100)
        //
        actualvolume = currNode.toElement().text().toInt();
        qDebug() << "current volume: " << actualvolume;
      }
      else if ( currName == QLatin1String( "muteenabled" ) )
      {
        //
        // kann gerät stumm gestellt werden?
        //
        if ( currName == QLatin1String( "true" ) )
        {
          muteenabled = true;
        }
        qDebug() << "mute enabled: " << muteenabled;
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
   * @brief HttpVolumeObject::~HttpVolumeObject
   */
  HttpVolumeObject::~HttpVolumeObject()
  {
    qDebug() << "...";
  }

  //
  // GETTER
  //

  int HttpVolumeObject::getTargetVolume() const
  {
    return targetvolume;
  }

  int HttpVolumeObject::getActualVolume() const
  {
    return actualvolume;
  }

  bool HttpVolumeObject::getMuteEnabled() const
  {
    return muteenabled;
  }

}  // namespace bose_soundtoch_lib
