#include "xmlparser/httpresponse/httpbassobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpBassObject::HttpBassObject
   * @param xmlreader
   * @param parent
   */
  HttpBassObject::HttpBassObject( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "bass" ) );
    resultType = ResultobjectType::R_BASS;
    //
    // Device ID finden (Attribute von <info>)
    //
    qDebug() << "...";
    deviceId = IResponseObject::getAttribute( reader, QLatin1String( "deviceID" ) );
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( IResponseObject::getNextStartTag( reader ) )
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
        qDebug() << "bass target is " << targetbass;
      }
      else if ( reader->name() == QLatin1String( "actualbass" ) )
      {
        //
        // Bass aktuelle einstellung
        //
        actualbass = reader->readElementText().toInt();
        qDebug() << "bass is actual " << actualbass;
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

  HttpBassObject::~HttpBassObject()
  {
    qDebug() << "...";
  }

  //
  // GETTER
  //

  int HttpBassObject::getTargetBass() const
  {
    return targetbass;
  }

  int HttpBassObject::getActualBass() const
  {
    return actualbass;
  }

}  // namespace bose_soundtoch_lib
