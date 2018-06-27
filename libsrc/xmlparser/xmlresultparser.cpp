#include "xmlresultparser.hpp"

namespace bose_soundtoch_lib
{
  XmlResultParser::XmlResultParser( std::shared_ptr< Logger > logger, QString &xmlString, QObject *parent )
      : QObject( parent )
      , lg( logger )
      , reader( std::unique_ptr< QXmlStreamReader >( new QXmlStreamReader( xmlString ) ) )
      , responseObject( nullptr )
  {
    lg->debug( "XmlResultParser::XmlResultParser: start parsing xml string..." );
    parseFile();
    //
    // Errorhandling
    //
    if ( reader->hasError() )
    {
      lg->crit( "XmlResultParser::XmlResultParser: error while parsing xml..." );
      // TODO: Errorhandling
    }
    else
    {
      lg->debug( "XmlResultParser::XmlResultParser: finishedt parsing xml string" );
    }
  }

  XmlResultParser::~XmlResultParser()
  {
    lg->debug( "XmlResultParser::~XmlResultParser..." );
  }

  bool XmlResultParser::hasError( void )
  {
    return ( reader->hasError() );
  }

  QString XmlResultParser::getErrorString( void )
  {
    if ( reader->hasError() )
    {
      QString errStr( QString( "%1 at line %2" ).arg( reader->errorString() ).arg( reader->lineNumber() ) );
      return ( errStr );
    }
    return ( QLatin1String( "no error" ) );
  }

  std::shared_ptr< IResponseObject > XmlResultParser::getResultObject( void )
  {
    return ( responseObject );
  }

  bool XmlResultParser::parseFile( void )
  {
    QStringRef rootelemName;
    // while ( !reader->atEnd() && !reader->hasError() )
    if ( !reader->atEnd() && !reader->hasError() )
    {
      // lese nächstes Element
      QXmlStreamReader::TokenType token = reader->readNext();
      //
      // Nur Start -> nächstes Element lesen
      //
      if ( token == QXmlStreamReader::StartDocument )
      {
        // weiter!
        reader->readNext();
      }
      //
      // das nächste element bearbeiten
      //
      rootelemName = reader->name();
      //
      // zuordnung machen, IResponseObject ist polymorph, das erleichtert die Handhabung
      //
      if ( rootelemName == QLatin1String( "info" ) )
      {
        //
        // Device INFO erhalten
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpDeviceInfoObject( lg, reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "nowPlaying" ) )
      {
        //
        // now playing erhalten
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpNowPlayingObject( lg, reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "volume" ) )
      {
        //
        // volume erhalten
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpVolumeObject( lg, reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "sources" ) )
      {
        //
        // sources erhalten
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpSourcesObject( lg, reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "zone" ) )
      {
        //
        // zoneninfo erhalten
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpZoneObject( lg, reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "bassCapabilities" ) )
      {
        //
        // bass möglichkeiten erhalten
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpBassCapabilitiesObject( lg, reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "bass" ) )
      {
        //
        // bass einstellungen erhalten
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpBassObject( lg, reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "presets" ) )
      {
        //
        // preset einstellungen erhalten
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpPresetsObject( lg, reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "group" ) )
      {
        //
        // Gruppeneinstellungen (nur SoundTouch10)
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpGroupObject( lg, reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "status" ) )
      {
        //
        // alles OK bei "set" Funktion
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpResultOkObject( lg, reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "errors" ) )
      {
        //
        // FEHLER bei "set" Funktion
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpResultErrorObject( lg, reader.get(), this ) );
      }
      else
      {
        //
        // hier weiss ich auch nicht weiter, der TAG ist unbekannt
        //
        lg->warn( QString( "XmlResultParser::parseFile: unsupported TAG in response XML struct found: %1" )
                      .arg( reader->name().toString() ) );
        while ( reader->readNextStartElement() && !reader->hasError() && !reader->atEnd() )
          ;
        /*
        while ( !reader->atEnd() && !reader->hasError() )
        {
          reader->readNext();
        }
        */
      }
    }
    //
    // gab es fehler, nur Fehler melden
    //
    if ( reader->hasError() )
      return ( true );
    //
    // Fehlerfrei, datren resetten
    //
    reader->clear();
    return ( false );
  }
}  // namespace bose_soundtoch_lib
