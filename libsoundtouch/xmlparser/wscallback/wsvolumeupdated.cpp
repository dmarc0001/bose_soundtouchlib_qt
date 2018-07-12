#include "wsvolumeupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsVolumeUpdated::WsVolumeUpdated
   * @param xmlreader
   * @param parent
   */
  WsVolumeUpdated::WsVolumeUpdated( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "volumeUpdated" ) );
    resultType = ResultobjectType::U_VOLUME;
    qDebug() << "...";
    //
    // wenn ein Knoten mit dem Namen "volume" vorhanden ist
    //
    QDomNode rootNode( domElem->firstChild() );
    if ( !rootNode.isNull() && rootNode.nodeName() == QLatin1String( "volume" ) )
    {
      //
      // lese soweit neue Elemente vorhanden sind
      //
      QDomNodeList rootChildNodesList( rootNode.childNodes() );
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
        //
        // das nächste element bearbeiten, welches ist es?
        //
        if ( currName == QLatin1String( "targetvolume" ) )
        {
          //
          // zu erreichende Lautstärke (0..100)
          //
          targetvolume = currNode.toElement().text().toInt();
          qDebug() << "volume to reach: " << targetvolume;
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
          if ( currNode.toElement().text() == QLatin1String( "true" ) )
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
  }

  /**
   * @brief WsVolumeUpdated::~WsVolumeUpdated
   */
  WsVolumeUpdated::~WsVolumeUpdated()
  {
  }

  //
  // GETTER
  //

  int WsVolumeUpdated::getTargetVolume() const
  {
    return targetvolume;
  }

  int WsVolumeUpdated::getActualVolume() const
  {
    return actualvolume;
  }

  bool WsVolumeUpdated::getMuteEnabled() const
  {
    return muteenabled;
  }

}  // namespace bose_soundtoch_lib
