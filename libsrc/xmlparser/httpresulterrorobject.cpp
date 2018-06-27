#include "httpresulterrorobject.hpp"

namespace bose_soundtoch_lib
{
  HttpResultErrorObject::HttpResultErrorObject( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "errors" ) );
    resultType = ResultobjectType::R_ERROR;
    //
    // ID finden (Attribute von <group>)
    //
    qDebug() << "...";
    deviceId = getAttibute( reader, QLatin1String( "deviceID" ) );
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      //
      // das nächste element bearbeiten, welches ist es? Eigentlich nur error
      //
      if ( reader->name() == QLatin1String( "error" ) )
      {
        //
        // einzelnen Fehler Parsen
        //
        parseError();
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag: " << reader->name().toString() << " -> " << reader->readElementText();
      }
    }
  }

  HttpResultErrorObject::~HttpResultErrorObject()
  {
    qDebug() << "...";
  }

  void HttpResultErrorObject::parseError( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "error" ) );
    //
    // einzelnen Fehler parsen und speichern
    //
    ResponseError error;
    qDebug() << "...";
    error.value = getAttibute( reader, QLatin1String( "value" ) ).toInt();
    error.name = getAttibute( reader, QLatin1String( "name" ) );
    error.severity = getAttibute( reader, QLatin1String( "severity" ) );
    //
    // den Fehlertext lesen
    //
    error.text = reader->readElementText();
  }

  QVector< ResponseError > HttpResultErrorObject::getErrors() const
  {
    return errors;
  }

}  // namespace bose_soundtoch_lib
