#include "wsvolumeupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsVolumeUpdated::WsVolumeUpdated
   * @param xmlreader
   * @param parent
   */
  WsVolumeUpdated::WsVolumeUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "volumeUpdated" ) );
    resultType = ResultobjectType::U_VOLUME;
    qDebug() << "...";
    //
    if ( reader->readNextStartElement() && !reader->hasError() )
    {
      //
      // das nächste element bearbeiten, welches ist es? Eigentlich nur volume
      //
      if ( reader->name() == QLatin1String( "volume" ) )
      {
        qDebug() << "tag <volume> found...";
        //
        // alles unterhalb "volume" lesen
        //
        while ( reader->readNextStartElement() && !reader->hasError() )
        {
          //
          // das nächste element bearbeiten, welches ist es?
          //
          if ( reader->name() == QLatin1String( "targetvolume" ) )
          {
            //
            // zu erreichende Lautstärke (0..100)
            //
            targetvolume = reader->readElementText().toInt();
            qDebug() << "volume to reach: " << targetvolume;
          }
          else if ( reader->name() == QLatin1String( "actualvolume" ) )
          {
            //
            // aktuelle Lautstärke (0..100)
            //
            actualvolume = reader->readElementText().toInt();
            qDebug() << "current volume: " << actualvolume;
          }
          else if ( reader->name() == QLatin1String( "muteenabled" ) )
          {
            //
            // kann gerät stumm gestellt werden?
            //
            if ( reader->readElementText() == QLatin1String( "true" ) )
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
            qWarning() << "unsupported tag: " << reader->name().toString() << " --> " << reader->readElementText();
          }
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
