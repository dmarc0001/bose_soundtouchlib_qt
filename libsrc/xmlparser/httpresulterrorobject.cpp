#include "httpresulterrorobject.hpp"

namespace radio
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
    QXmlStreamAttributes attr = reader->attributes();
    if ( attr.hasAttribute( QLatin1String( "deviceId" ) ) )
    {
      deviceId = attr.value( QLatin1String( "deviceId" ) ).toString();
      lg->debug( QString( "ResultErrorObject::ResultErrorObject: attribute \"deviceId\" has value %1" ).arg( deviceId ) );
    }
    else
    {
      lg->warn( "ResultErrorObject::ResultErrorObject: there is no attribute \"deviceId\"..." );
    }
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
    QXmlStreamAttributes attr = reader->attributes();
    if ( attr.hasAttribute( QLatin1String( "value" ) ) )
    {
      error.value = attr.value( QLatin1String( "value" ) ).toInt();
      lg->debug( QString( "ResultErrorObject::parseError: attribute \"value\" has value %1" ).arg( error.value ) );
    }
    else
    {
      lg->warn( "ResultErrorObject::parseError: there is no attribute \"value\"..." );
    }
    if ( attr.hasAttribute( QLatin1String( "name" ) ) )
    {
      error.name = attr.value( QLatin1String( "name" ) ).toString();
      lg->debug( QString( "ResultErrorObject::parseError: attribute \"name\" has value %1" ).arg( error.name ) );
    }
    else
    {
      lg->warn( "ResultErrorObject::parseError: there is no attribute \"name\"..." );
    }
    if ( attr.hasAttribute( QLatin1String( "severity" ) ) )
    {
      error.severity = attr.value( QLatin1String( "severity" ) ).toString();
      lg->debug( QString( "ResultErrorObject::parseError: attribute \"severity\" has value %1" ).arg( error.severity ) );
    }
    else
    {
      lg->warn( "ResultErrorObject::parseError: there is no attribute \"severity\"..." );
    }
    //
    // den Fehlertext lesen
    //
    error.text = reader->readElementText();
  }

  QString HttpResultErrorObject::getDeviceId() const
  {
    return deviceId;
  }

  QVector< ResponseError > HttpResultErrorObject::getErrors() const
  {
    return errors;
  }

}  // namespace radio
