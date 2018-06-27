#include "httpbasscapabilitiesobject.hpp"

namespace bose_soundtoch_lib
{
  HttpBassCapabilitiesObject::HttpBassCapabilitiesObject( QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "bassCapabilities" ) );
    resultType = ResultobjectType::R_BASS_CAPABILITIES;
    //
    // Device ID finden (Attribute von <info>)
    //
    qDebug() << "...";
    deviceId = getAttibute( reader, QLatin1String( "deviceID" ) );
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
      }
      else if ( reader->name() == QLatin1String( "bassMin" ) )
      {
        bassMin = reader->readElementText().toInt();
      }
      else if ( reader->name() == QLatin1String( "bassMax" ) )
      {
        bassMax = reader->readElementText().toInt();
      }
      else if ( reader->name() == QLatin1String( "bassDefault" ) )
      {
        bassDefault = reader->readElementText().toInt();
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

  HttpBassCapabilitiesObject::~HttpBassCapabilitiesObject()
  {
    qDebug() << "...";
  }

  bool HttpBassCapabilitiesObject::getBassAvailable() const
  {
    return bassAvailable;
  }

  int HttpBassCapabilitiesObject::getBassMin() const
  {
    return bassMin;
  }

  int HttpBassCapabilitiesObject::getBassMax() const
  {
    return bassMax;
  }

  int HttpBassCapabilitiesObject::getBassDefault() const
  {
    return bassDefault;
  }

}  // namespace bose_soundtoch_lib
