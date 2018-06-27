#include "wsvolumeupdated.hpp"

namespace bose_soundtoch_lib
{
  WsVolumeUpdated::WsVolumeUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "volumeUpdated" ) );
    resultType = ResultobjectType::U_VOLUME;
    lg->debug( "WsVolumeUpdate::WsVolumeUpdate..." );
    //
    if ( reader->readNextStartElement() && !reader->hasError() )
    {
      //
      // das nächste element bearbeiten, welches ist es? Eigentlich nur presets
      //
      if ( reader->name() == QLatin1String( "volume" ) )
      {
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
            lg->debug( QString( "WsVolumeUpdate::WsVolumeUpdate: volume to reach: %1" ).arg( targetvolume ) );
          }
          else if ( reader->name() == QLatin1String( "actualvolume" ) )
          {
            //
            // aktuelle Lautstärke (0..100)
            //
            actualvolume = reader->readElementText().toInt();
            lg->debug( QString( "WsVolumeUpdate::WsVolumeUpdate: current volume: %1" ).arg( actualvolume ) );
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
            lg->debug( QString( "WsVolumeUpdate::WsVolumeUpdate: mute enabled: %1" ).arg( muteenabled ) );
          }
          else
          {
            //
            // unsupportet elements
            //
            lg->debug( QString( "WsVolumeUpdate::WsVolumeUpdate: %1 -> %2" )
                           .arg( reader->name().toString() )
                           .arg( reader->readElementText() ) );
          }
        }
      }
    }
  }

  WsVolumeUpdated::~WsVolumeUpdated()
  {
  }

  int WsVolumeUpdated::getVolume() const
  {
    return volume;
  }

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

}  // namespace radio
