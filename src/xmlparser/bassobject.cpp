#include "bassobject.hpp"

namespace radio
{
  BassObject::BassObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "bass" ) );
    resultType = ResultobjectType::R_BASS;
    //
    // Device ID finden (Attribute von <info>)
    //
    lg->debug( "BassObject::BassObject..." );
    lg->debug( "BassObject::BassObject: check for attribute in \"bass\"..." );
    QXmlStreamAttributes attr = reader->attributes();
    if ( attr.hasAttribute( QLatin1String( "deviceID" ) ) )
    {
      deviceId = attr.value( QLatin1String( "deviceID" ) ).toString();
      lg->debug( QString( "BassObject::BassObject: attribute \"deviceID\" has value %1" ).arg( deviceId ) );
    }
    else
    {
      lg->warn( "SourcesObject::SourcesObject: there is no attribute \"deviceID\"..." );
    }
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

  BassObject::~BassObject()
  {
    lg->debug( "BassObject::~BassObject..." );
  }
}  // namespace radio
