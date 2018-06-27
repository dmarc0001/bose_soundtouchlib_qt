#include "httpresulterrorobject.hpp"

namespace bose_soundtoch_lib
{
  HttpResultErrorObject::HttpResultErrorObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "errors" ) );
    resultType = ResultobjectType::R_ERROR;
    //
    // ID finden (Attribute von <group>)
    //
    lg->debug( "ResultErrorObject::ResultErrorObject..." );
    lg->debug( "ResultErrorObject::ResultErrorObject: check for attributes in \"errors\"..." );
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
        lg->debug( QString( "ResultErrorObject::ResultErrorObject: %1 -> %2" )
                       .arg( reader->name().toString() )
                       .arg( reader->readElementText() ) );
      }
    }
  }

  HttpResultErrorObject::~HttpResultErrorObject()
  {
    lg->debug( "ResultErrorObject::~ResultErrorObject..." );
  }

  void HttpResultErrorObject::parseError( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "error" ) );
    //
    // einzelnen Fehler parsen und speichern
    //
    ResponseError error;
    lg->debug( "ResultErrorObject::parseError..." );
    lg->debug( "ResultErrorObject::parseError: check for attributes in \"error\"..." );
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

}  // namespace radio
