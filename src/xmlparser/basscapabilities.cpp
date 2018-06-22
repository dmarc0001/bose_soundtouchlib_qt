#include "basscapabilities.hpp"

namespace radio
{
  BassCapabilities::BassCapabilities( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "bassCapabilities" ) );
    resultType = ResultobjectType::R_BASS_CAPABILITIES;
    //
    // Device ID finden (Attribute von <info>)
    //
    lg->debug( "BassCapabilities::BassCapabilities..." );
    lg->debug( "BassCapabilities::BassCapabilities: check for attribute \"deviceID\"..." );
    QXmlStreamAttributes attr = reader->attributes();
    if ( attr.hasAttribute( QLatin1String( "deviceID" ) ) )
    {
      deviceId = attr.value( QLatin1String( "deviceID" ) ).toString();
      lg->debug( QString( "BassCapabilities::BassCapabilities: attribute \"deviceID\" has value %1" ).arg( deviceId ) );
    }
    else
    {
      lg->warn( "BassCapabilities::BassCapabilities: there is no attribute \"deviceID\"..." );
    }
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      //
      // das nächste element bearbeiten, welches ist es?
      //
      if ( reader->name() == QLatin1String( "bassAvailable" ) )
      {
        if ( reader->readElementText() == QLatin1String( "true" ) )
        {
          bassAvailable = true;
        }
        lg->debug( QString( "BassCapabilities::BassCapabilities: bassAvailible: %1" ).arg( bassAvailable ) );
      }
      else if ( reader->name() == QLatin1String( "bassMin" ) )
      {
        bassMin = reader->readElementText().toInt();
        lg->debug( QString( "BassCapabilities::BassCapabilities: bassMin: %1" ).arg( bassMin ) );
      }
      else if ( reader->name() == QLatin1String( "bassMax" ) )
      {
        bassMax = reader->readElementText().toInt();
        lg->debug( QString( "BassCapabilities::BassCapabilities: bassMax: %1" ).arg( bassMax ) );
      }
      else if ( reader->name() == QLatin1String( "bassDefault" ) )
      {
        bassDefault = reader->readElementText().toInt();
        lg->debug( QString( "BassCapabilities::BassCapabilities: bassDefault: %1" ).arg( bassDefault ) );
      }
      else
      {
        //
        // unsupportet elements
        //
        lg->debug( QString( "BassCapabilities::BassCapabilities: %1 -> %2" )
                       .arg( reader->name().toString() )
                       .arg( reader->readElementText() ) );
      }
    }
  }

  BassCapabilities::~BassCapabilities()
  {
    lg->debug( "BassCapabilities::~BassCapabilities" );
  }
}  // namespace radio
