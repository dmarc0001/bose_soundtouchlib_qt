#include "xmlparser/httpresponse/httpresulterrorobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpResultErrorObject::HttpResultErrorObject
   * @param xmlreader
   * @param parent
   */
  HttpResultErrorObject::HttpResultErrorObject( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "errors" ) );
    resultType = ResultobjectType::R_ERROR;
    //
    // ID finden (Attribute von <group>)
    //
    qDebug() << "...";
    deviceId = IResponseObject::getAttribute( reader, QLatin1String( "deviceID" ) );
    qDebug() << "device id: " << deviceId;
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( IResponseObject::getNextStartTag( reader ) )
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

  /**
   * @brief HttpResultErrorObject::~HttpResultErrorObject
   */
  HttpResultErrorObject::~HttpResultErrorObject()
  {
    qDebug() << "...";
  }

  /**
   * @brief HttpResultErrorObject::parseError
   */
  void HttpResultErrorObject::parseError( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "error" ) );
    //
    // einzelnen Fehler parsen und speichern
    //
    ResponseError error;
    qDebug() << "...";
    error.value = IResponseObject::getAttribute( reader, QLatin1String( "value" ) ).toInt();
    qDebug() << "error value: " << error.value;
    error.name = IResponseObject::getAttribute( reader, QLatin1String( "name" ) );
    qDebug() << "error name: " << error.name;
    error.severity = IResponseObject::getAttribute( reader, QLatin1String( "severity" ) );
    qDebug() << " error severity: " << error.severity;
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
