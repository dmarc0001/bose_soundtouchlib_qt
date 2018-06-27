#include "xmlupdateparser.hpp"

namespace bose_soundtoch_lib
{
  XMLUpdateParser::XMLUpdateParser( std::shared_ptr< Logger > logger, QString &xmlString, QObject *parent )
      : QObject( parent )
      , lg( logger )
      , reader( std::unique_ptr< QXmlStreamReader >( new QXmlStreamReader( xmlString ) ) )
      , responseObject( nullptr )
  {
    lg->debug( "XMLUpdateParser::XMLUpdateParser: start parsing xml string..." );
    parseFile();
    //
    // Errorhandling
    //
    if ( reader->hasError() )
    {
      lg->crit( "XMLUpdateParser::XMLUpdateParser: error while parsing xml..." );
      // TODO: Errorhandling
    }
    else
    {
      lg->debug( "XMLUpdateParser::XMLUpdateParser: finishedt parsing xml string" );
    }
  }

  XMLUpdateParser::~XMLUpdateParser()
  {
    lg->debug( "XMLUpdateParser::~XMLUpdateParser..." );
  }

  bool XMLUpdateParser::hasError( void )
  {
    return ( reader->hasError() );
  }

  QString XMLUpdateParser::getErrorString( void )
  {
    if ( reader->hasError() )
    {
      QString errStr( QString( "%1 at line %2" ).arg( reader->errorString() ).arg( reader->lineNumber() ) );
      return ( errStr );
    }
    return ( QLatin1String( "no error" ) );
  }

  std::shared_ptr< IResponseObject > XMLUpdateParser::getResultObject( void )
  {
    return ( responseObject );
  }

  bool XMLUpdateParser::parseFile( void )
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
      // SDK-Info : <SoundTouchSdkInfo serverVersion="4" serverBuild="trunk r42017 v4 epdbuild cepeswbld02" />

      if ( rootelemName == QLatin1String( "SoundTouchSdkInfo" ) )
      {
        //
        // SDK Info (beim Verbinden übertragen
        // erzeuge das Objekt und Parse es
        //
        responseObject = std::shared_ptr< IResponseObject >( new WsSoundTouchSdkInfo( lg, reader.get(), this ) );
      }
      else if ( rootelemName == QLatin1String( "userActivityUpdate" ) )
      {
        //
        // Benutzer aktiv geändert
        // Activiti update ist NICHT in <updates>
        responseObject = std::shared_ptr< IResponseObject >( new WsUserActivityUpdated( lg, reader.get(), this ) );
        // responseObject->setDeviceId( deviceId );
      }

      else if ( rootelemName == QLatin1String( "updates" ) )
      {
        QString deviceId = getAttibute( reader.get(), QLatin1String( "deviceId" ) );
        //
        // eine UPDATE Meldung, sollte alles sein ausser SoundTouchSdkInfo
        //
        if ( reader->readNextStartElement() && !reader->hasError() )
        {
          QStringRef updateElementName = reader->name();
          //
          // jetzt entscheidet sich welche Art UPDATE empfangen wurde
          //
          if ( updateElementName == QLatin1String( "presetsUpdates" ) )
          {
            responseObject = std::shared_ptr< IResponseObject >( new WsPresetUpdateObject( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // nowPlayingUpdate
          else if ( updateElementName == QLatin1String( "nowPlayingUpdated" ) )
          {
            //
            // ich kann hier das HttpNowPlayingObjekt recyclen (via "using" in der Headerdatei)
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsNowPlayingUpdate( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // nowSelection
          else if ( updateElementName == QLatin1String( "nowSelectionUpdated" ) )
          {
            //
            // Selection geändert
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsNowSelectionUpdated( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // volume geändert
          else if ( updateElementName == QLatin1String( "volumeUpdated" ) )
          {
            //
            // Lautstärke geändert
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsVolumeUpdated( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // Bass geändert
          else if ( updateElementName == QLatin1String( "bassUpdated" ) )
          {
            //
            // Bass geändert
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsBassUpdated( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // Zone geändert
          else if ( updateElementName == QLatin1String( "zoneUpdated" ) )
          {
            //
            // Zonenmitgliedschaft geändert
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsZoneUpdated( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // Info geändert
          else if ( updateElementName == QLatin1String( "infoUpdated" ) )
          {
            //
            // Geräteinfo geändert
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsInfoUpdated( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // Gerätename geändert
          else if ( updateElementName == QLatin1String( "nameUpdated" ) )
          {
            //
            // Gerätename geändert
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsNameUpdated( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // error update
          else if ( updateElementName == QLatin1String( "errorUpdate" ) )
          {
            //
            // Fehlermeldung geändert
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsErrorUpdated( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // goup update
          else if ( updateElementName == QLatin1String( "groupUpdated" ) )
          {
            //
            // Gruppe (SounfTouch 10 only) geändert
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsGroupUpdated( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          //
          // UNSUPPORTED Featuree
          //
          // browse
          else if ( updateElementName == QLatin1String( "browseUpdated" ) )
          {
            //
            // Browsing update
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsBrowseUpdated( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // recents
          else if ( updateElementName == QLatin1String( "recentsUpdated" ) )
          {
            //
            // neuigkeiten update
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsRecentsUpdated( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // source updated
          else if ( updateElementName == QLatin1String( "sourcesUpdated" ) )
          {
            //
            // Sources update
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsSourceUpdated( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // language update
          else if ( updateElementName == QLatin1String( "languageUpdated" ) )
          {
            //
            // Sprache geändert
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsLanguageUpdated( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // user activity update
          // ist aktuell 2018-07 direkt root tag!
          else if ( updateElementName == QLatin1String( "userActivityUpdate" ) )
          {
            //
            // Benutzer aktiv geändert
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsUserActivityUpdated( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // user inactivity
          else if ( updateElementName == QLatin1String( "userInactivityUpdate" ) )
          {
            //
            // Benutzer inaktiv geändert
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsUserInactivityUpdated( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // connection state
          else if ( updateElementName == QLatin1String( "connectionStateUpdated" ) )
          {
            //
            // Verbindungsstatus geändert
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsConnectionStateUpdated( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // audioproducttonecontrols
          else if ( updateElementName == QLatin1String( "audioproducttonecontrols" ) )
          {
            //
            // BOSE intern
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsAudioProductToneControls( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // audioproductlevelcontrols
          else if ( updateElementName == QLatin1String( "audioproductlevelcontrols" ) )
          {
            //
            // BOSE intern
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsAudioProductLevelControls( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          // audiospcontrols
          else if ( updateElementName == QLatin1String( "audiospcontrols" ) )
          {
            //
            // BOSE intern
            //
            responseObject = std::shared_ptr< IResponseObject >( new WsAudioSpControls( lg, reader.get(), this ) );
            responseObject->setDeviceId( deviceId );
          }
          else
          {
            //
            // hier weiss ich auch nicht weiter, der TAG ist unbekannt
            //
            lg->warn( QString( "XMLUpdateParser::parseFile: unsupported TAG behind <updates> in response XML struct found: %1" )
                          .arg( reader->name().toString() ) );
          }
          // den Rest weg lesen
          while ( reader->readNextStartElement() && !reader->hasError() && !reader->atEnd() )
            ;
        }
      }
      else
      {
        //
        // hier weiss ich auch nicht weiter, der TAG ist unbekannt
        //
        lg->warn( QString( "XMLUpdateParser::parseFile: unsupported TAG in response XML struct found: %1" )
                      .arg( reader->name().toString() ) );
        while ( !reader->atEnd() && !reader->hasError() )
        {
          reader->readNext();
        }
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

  QString XMLUpdateParser::getAttibute( QXmlStreamReader *reader, QLatin1String name ) const
  {
    //
    // attribute finden
    //
    QString result;
    QXmlStreamAttributes attr = reader->attributes();
    if ( attr.hasAttribute( name ) )
    {
      result = attr.value( name ).toString();
    }
    else
    {
      result.clear();
    }
    return ( result );
  }

}  // namespace bose_soundtoch_lib
