#include "httpvolumeobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpVolumeObject::HttpVolumeObject
   * @param xmlreader
   * @param parent
   */
  HttpVolumeObject::HttpVolumeObject( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "volume" ) );
    resultType = ResultobjectType::R_VOLUME;
    //
    // Device ID finden (Attribute von <volume>)
    //
    qDebug() << "...";
    deviceId = IResponseObject::getAttribute( reader, QLatin1String( "deviceID" ) );
    qDebug() << "device: " << deviceId;
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( IResponseObject::getNextStartTag( reader ) )
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
        qDebug() << "target volume: " << targetvolume;
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
