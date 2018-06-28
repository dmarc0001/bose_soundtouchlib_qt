#include "httpbasscapabilitiesobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpBassCapabilitiesObject::HttpBassCapabilitiesObject Objekt parsen
   * @param xmlreader
   * @param parent
   */
  HttpBassCapabilitiesObject::HttpBassCapabilitiesObject( QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "bassCapabilities" ) );
    resultType = ResultobjectType::R_BASS_CAPABILITIES;
    //
    // Device ID finden (Attribute von <info>)
    //
    qDebug() << "...";
    deviceId = IResponseObject::getAttribute( reader, QLatin1String( "deviceID" ) );
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
          qDebug() << "bass availible: " << bassAvailable;
        }
      }
      else if ( reader->name() == QLatin1String( "bassMin" ) )
      {
        bassMin = reader->readElementText().toInt();
        qDebug() << "bass minimum value: " << bassMin;
      }
      else if ( reader->name() == QLatin1String( "bassMax" ) )
      {
        bassMax = reader->readElementText().toInt();
        qDebug() << "bass maximum value: " << bassMax;
      }
      else if ( reader->name() == QLatin1String( "bassDefault" ) )
      {
        bassDefault = reader->readElementText().toInt();
        qDebug() << "bass default value: " << bassDefault;
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
   * @brief HttpBassCapabilitiesObject::~HttpBassCapabilitiesObject
   */
  HttpBassCapabilitiesObject::~HttpBassCapabilitiesObject()
  {
    qDebug() << "...";
  }

  //
  // GETTER Methoden
  //

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
