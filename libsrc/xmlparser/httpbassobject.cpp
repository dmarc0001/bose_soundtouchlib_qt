#include "httpbassobject.hpp"

namespace bose_soundtoch_lib
{
  HttpBassObject::HttpBassObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "bass" ) );
    resultType = ResultobjectType::R_BASS;
    //
    // Device ID finden (Attribute von <info>)
    //
    lg->debug( "BassObject::BassObject..." );
    lg->debug( "BassObject::BassObject: check for attribute in \"bass\"..." );
    deviceId = getAttibute( reader, QLatin1String( "deviceID" ) );
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      //
      // das nächste element bearbeiten, welches ist es?
      //
      if ( reader->name() == QLatin1String( "targetbass" ) )
      {
        //
        // Bass zieleinstellung
        //
        targetbass = reader->readElementText().toInt();
        lg->debug( QString( "BassObject::BassObject: bass target is %1" ).arg( targetbass ) );
      }
      else if ( reader->name() == QLatin1String( "actualbass" ) )
      {
        //
        // Bass aktuelle einstellung
        //
        actualbass = reader->readElementText().toInt();
        lg->debug( QString( "BassObject::BassObject: bass is actual %1" ).arg( actualbass ) );
      }
      else
      {
        //
        // unsupportet elements
        //
        lg->debug( QString( "BassObject::BassObject: %1 -> %2" ).arg( reader->name().toString() ).arg( reader->readElementText() ) );
      }
    }
  }

  HttpBassObject::~HttpBassObject()
  {
    lg->debug( "BassObject::~BassObject..." );
  }

  int HttpBassObject::getTargetBass() const
  {
    return targetbass;
  }

  int HttpBassObject::getActualBass() const
  {
    return actualbass;
  }

}  // namespace radio
