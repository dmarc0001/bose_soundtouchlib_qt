#include "xmlparser/xmlresultparser.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief XmlResultParser::XmlResultParser
   * @param xmlString
   * @param parent
   */
  XmlResultParser::XmlResultParser( QString &xmlString, QObject *parent )
      : QObject( parent )
      , reader( std::unique_ptr< QXmlStreamReader >( new QXmlStreamReader( xmlString ) ) )
      , responseObject( nullptr )
  {
    qDebug() << "...";
    parseFile();
    //
    // Errorhandling
    //
    if ( reader->hasError() )
    {
      qCritical() << "error while parsing xml...";
      //
      // TODO: Errorhandling
      //
    }
    else
    {
      qDebug() << "finished parsing response xml string";
    }
  }

  /**
   * @brief XmlResultParser::~XmlResultParser
   */
  XmlResultParser::~XmlResultParser()
  {
    qDebug() << "...";
  }

  /**
   * @brief XmlResultParser::parseFile
   * @return
   */
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
        responseObject = std::shared_ptr< IResponseObject >( new HttpDeviceInfoObject( reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "nowPlaying" ) )
      {
        //
        // now playing erhalten
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpNowPlayingObject( reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "volume" ) )
      {
        //
        // volume erhalten
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpVolumeObject( reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "sources" ) )
      {
        //
        // sources erhalten
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpSourcesObject( reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "zone" ) )
      {
        //
        // zoneninfo erhalten
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpZoneObject( reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "bassCapabilities" ) )
      {
        //
        // bass möglichkeiten erhalten
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpBassCapabilitiesObject( reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "bass" ) )
      {
        //
        // bass einstellungen erhalten
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpBassObject( reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "presets" ) )
      {
        //
        // preset einstellungen erhalten
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpPresetsObject( reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "group" ) )
      {
        //
        // Gruppeneinstellungen (nur SoundTouch10)
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpGroupObject( reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "status" ) )
      {
        //
        // alles OK bei "set" Funktion
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpResultOkObject( reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "errors" ) )
      {
        //
        // FEHLER bei "set" Funktion
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new HttpResultErrorObject( reader.get(), this ) );
      }
      else
      {
        //
        // hier weiss ich auch nicht weiter, der TAG ist unbekannt
        //
        qWarning() << "unsupported tag in response XML struct found: " << reader->name().toString();
        // while ( reader->readNextStartElement() && !reader->hasError() && !reader->atEnd() );
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

  //
  // GETTER
  //

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

}  // namespace bose_soundtoch_lib
