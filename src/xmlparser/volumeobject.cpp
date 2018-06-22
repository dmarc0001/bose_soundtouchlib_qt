#include "volumeobject.hpp"

namespace radio
{
  VolumeObject::VolumeObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "volume" ) );
    resultType = ResultobjectType::R_VOLUME;
    //
    // Device ID finden (Attribute von <volume>)
    //
    lg->debug( "VolumeObject::VolumeObject..." );
    lg->debug( "VolumeObject::VolumeObject: check for attribute \"deviceID\"..." );
    QXmlStreamAttributes attr = reader->attributes();
    if ( attr.hasAttribute( QLatin1String( "deviceID" ) ) )
    {
      deviceId = attr.value( QLatin1String( "deviceID" ) ).toString();
      lg->debug( QString( "VolumeObject::VolumeObject: attribute \"deviceID\" has value %1" ).arg( deviceId ) );
    }
    else
    {
      lg->warn( "VolumeObject::VolumeObject: there is no attribute \"deviceID\"..." );
    }
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
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
        lg->debug( QString( "VolumeObject::VolumeObject: volume to reach: %1" ).arg( targetvolume ) );
      }
      else if ( reader->name() == QLatin1String( "actualvolume" ) )
      {
        //
        // aktuelle Lautstärke (0..100)
        //
        actualvolume = reader->readElementText().toInt();
        lg->debug( QString( "VolumeObject::VolumeObject: current volume: %1" ).arg( actualvolume ) );
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
        lg->debug( QString( "VolumeObject::VolumeObject: mute enabled: %1" ).arg( muteenabled ) );
      }
      else
      {
        //
        // unsupportet elements
        //
        lg->debug(
            QString( "VolumeObject::VolumeObject: %1 -> %2" ).arg( reader->name().toString() ).arg( reader->readElementText() ) );
      }
    }
  }

  VolumeObject::~VolumeObject()
  {
    lg->debug( "VolumeObject::~VolumeObject..." );
  }
}  // namespace radio
