#include "nowplayingobject.hpp"

namespace radio
{
  NowPlayingObject::NowPlayingObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "nowPlaying" ) );
    resultType = ResultobjectType::R_NOW_PLAYING;
    //
    // DeviceID/source finden (Attribute von <nowPlaying>)
    //
    lg->debug( "NowPlayingObject::NowPlayingObject..." );
    if ( reader->name() == QLatin1String( "nowPlaying" ) )
    {
      lg->debug( "NowPlayingObject::NowPlayingObject: check for attribute \"deviceID\"..." );
      QXmlStreamAttributes attr = reader->attributes();
      if ( attr.hasAttribute( QLatin1String( "deviceID" ) ) )
      {
        deviceId = attr.value( QLatin1String( "deviceID" ) ).toString();
        lg->debug( QString( "NowPlayingObject::NowPlayingObject: attribute \"deviceID\" has value %1" ).arg( deviceId ) );
      }
      else
      {
        lg->warn( "NowPlayingObject::NowPlayingObject: there is no attribute \"deviceID\"..." );
      }
      if ( attr.hasAttribute( QLatin1String( "source" ) ) )
      {
        source = attr.value( QLatin1String( "source" ) ).toString();
        lg->debug( QString( "NowPlayingObject::NowPlayingObject: attribute \"source\" has value %1" ).arg( source ) );
      }
      else
      {
        lg->warn( "NowPlayingObject::NowPlayingObject: there is no attribute \"source\"..." );
      }
    }
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      if ( reader->name() == QLatin1String( "ContentItem" ) )
      {
        //
        // Objet mit Attributen (kannman bei /select nutzen)
        //
        lg->debug( "NowPlayingObject::NowPlayingObject: ContentItem object detected." );
        parseContentItem();
      }
      else if ( reader->name() == QLatin1String( "track" ) )
      {
        //
        // gespielter track
        //
        track = reader->readElementText();
        lg->debug( QString( "NowPlayingObject::NowPlayingObject: track: %1" ).arg( track ) );
      }
      else if ( reader->name() == QLatin1String( "artist" ) )
      {
        //
        // gespielter künstler
        //
        artist = reader->readElementText();
        lg->debug( QString( "NowPlayingObject::NowPlayingObject: artist: %1" ).arg( artist ) );
      }
      else if ( reader->name() == QLatin1String( "album" ) )
      {
        //
        // gespieltes album
        //
        album = reader->readElementText();
        lg->debug( QString( "NowPlayingObject::NowPlayingObject: album: %1" ).arg( album ) );
      }
      else if ( reader->name() == QLatin1String( "genre" ) )
      {
        //
        // gespieltes genre
        //
        genre = reader->readElementText();
        lg->debug( QString( "NowPlayingObject::NowPlayingObject: genre: %1" ).arg( genre ) );
      }
      else if ( reader->name() == QLatin1String( "stationName" ) )
      {
        //
        // gespielte station/playlist
        //
        stationName = reader->readElementText();
        lg->debug( QString( "NowPlayingObject::NowPlayingObject: station or playlist: %1" ).arg( stationName ) );
      }
      else if ( reader->name() == QLatin1String( "art" ) )
      {
        //
        // Dekoration für Sender/Playlist (Logo etc)
        //
        parseArt();
      }
      else if ( reader->name() == QLatin1String( "time" ) )
      {
        //
        // Laufzeit
        //
        parseTime();
      }
      else if ( reader->name() == QLatin1String( "skipEnabled" ) )
      {
        //
        // weiter springen möglich?
        //
        skipEnabled = true;
        lg->debug( "NowPlayingObject::NowPlayingObject: skip enabled." );
      }
      else if ( reader->name() == QLatin1String( "skipPreviousEnabled" ) )
      {
        //
        // zurück springen möglich?
        //
        skipEnabled = true;
        lg->debug( "NowPlayingObject::NowPlayingObject: skip previous enabled." );
      }
      else if ( reader->name() == QLatin1String( "skipPreviousSupported" ) )
      {
        //
        // zurück springen unterstützt?
        //
        QXmlStreamAttributes attr = reader->attributes();
        if ( attr.hasAttribute( "value" ) )
        {
          if ( attr.value( QLatin1String( "value" ) ) == QLatin1String( "true" ) )
          {
            skipPreviousSupported = true;
          }
          lg->debug( QString( "NowPlayingObject::NowPlayingObject: skip previous supported:" ).arg( skipPreviousSupported ) );
        }
      }
      else if ( reader->name() == QLatin1String( "favoriteEnabled" ) )
      {
        //
        // als favorite möglich?
        //
        favoriteEnabled = true;
        lg->debug( "NowPlayingObject::NowPlayingObject: favoriteEnabled enabled." );
      }
      else if ( reader->name() == QLatin1String( "isFavorite" ) )
      {
        //
        // als favorite möglich?
        //
        isFavorite = true;
        lg->debug( "NowPlayingObject::NowPlayingObject: isFavorite enabled." );
      }
      else if ( reader->name() == QLatin1String( "rateEnabled" ) )
      {
        //
        // bewertung möglich?
        //
        rateEnabled = true;
        lg->debug( "NowPlayingObject::NowPlayingObject: rateEnabled enabled." );
      }
      else if ( reader->name() == QLatin1String( "rating" ) )
      {
        //
        // Bewertng (zweideutig in der Doku)
        //
        rating = reader->readElementText();
        lg->debug( QString( "NowPlayingObject::NowPlayingObject: rating: %1" ).arg( rating ) );
      }
      else if ( reader->name() == QLatin1String( "playStatus" ) )
      {
        //
        // welcher Status beim abspielen
        //
        playStatus = reader->readElementText();
        lg->debug( QString( "NowPlayingObject::NowPlayingObject: playStatus: %1" ).arg( playStatus ) );
      }
      else if ( reader->name() == QLatin1String( "shuffleSettings" ) )
      {
        //
        // zufallabspielen einstellungen
        //
        shuffleSettings = reader->readElementText();
        lg->debug( QString( "NowPlayingObject::NowPlayingObject: shuffleSettings: %1" ).arg( shuffleSettings ) );
      }
      else if ( reader->name() == QLatin1String( "repeatSettings" ) )
      {
        //
        // Wiederholungseinstellungen
        //
        repeatSettings = reader->readElementText();
        lg->debug( QString( "NowPlayingObject::NowPlayingObject: repeatSettings: %1" ).arg( repeatSettings ) );
      }
      else if ( reader->name() == QLatin1String( "streamType" ) )
      {
        //
        // Art des Streams
        //
        streamType = reader->readElementText();
        lg->debug( QString( "NowPlayingObject::NowPlayingObject: streamType: %1" ).arg( streamType ) );
      }
      else if ( reader->name() == QLatin1String( "description" ) )
      {
        //
        // Beschreibung des Streams
        //
        _description = reader->readElementText();
        lg->debug( QString( "NowPlayingObject::NowPlayingObject: stream description: %1" ).arg( _description ) );
      }
      else if ( reader->name() == QLatin1String( "stationLocation" ) )
      {
        //
        // station standort (internet only)
        //
        stationLocation = reader->readElementText();
        lg->debug( QString( "NowPlayingObject::NowPlayingObject: station location: %1" ).arg( stationLocation ) );
      }
      else if ( reader->name() == QLatin1String( "connectionStatusInfo" ) )
      {
        parseConnectionStatusInfo();
      }
      else
      {
        //
        // unsupportet elements
        //
        lg->debug( QString( "NowPlayingObject::NowPlayingObject: %1 -> %2" )
                       .arg( reader->name().toString() )
                       .arg( reader->readElementText() ) );
      }
    }
  }

  NowPlayingObject::~NowPlayingObject()
  {
    lg->debug( "NowPlayingObject::~NowPlayingObject..." );
  }

  void NowPlayingObject::parseConnectionStatusInfo( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "time" ) );
    //
    // Spielzeit
    //
    lg->debug( "NowPlayingObject::parseConnectionStatusInfo..." );
    while ( reader->readNextStartElement() )
    {
      //
      // welchen Eintrag hab ich gefunden?
      //
      if ( reader->name() == QLatin1String( "status" ) )
      {
        nowPlayingConnectStatusInfo.status = reader->readElementText();
        lg->debug( QString( "NowPlayingObject::parseConnectionStatusInfo: status: %1" ).arg( nowPlayingConnectStatusInfo.status ) );
      }
      else if ( reader->name() == QLatin1String( "deviceName" ) )
      {
        nowPlayingConnectStatusInfo.deviceName = reader->readElementText();
        lg->debug(
            QString( "NowPlayingObject::parseConnectionStatusInfo: device name: %1" ).arg( nowPlayingConnectStatusInfo.deviceName ) );
      }
      else
      {
        lg->warn( QString( "NowPlayingObject::parseContentItem: unsupportet entry %1 -> %2 " )
                      .arg( reader->name().toString() )
                      .arg( reader->readElementText() ) );
      }
    }
  }

  void NowPlayingObject::parseTime( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "time" ) );
    //
    // Spielzeit
    //
    lg->debug( "NowPlayingObject::parseTime..." );
    if ( reader->name() == QLatin1String( "time" ) )
    {
      //
      // Attribut "total"
      //
      lg->debug( "NowPlayingObject::parseTime: check for attribute \"total\"..." );
      QXmlStreamAttributes attr = reader->attributes();
      if ( attr.hasAttribute( QLatin1String( "total" ) ) )
      {
        nowPlayingTime.total_sec = attr.value( QLatin1String( "total" ) ).toInt();
        lg->debug( QString( "NowPlayingObject::parseTime: attribute \"total\" has value %1" ).arg( nowPlayingTime.total_sec ) );
      }
      else
      {
        lg->warn( "NowPlayingObject::parseTime: there is no attribute \"total\"..." );
      }
      //
      // jetzt den Inhalt
      //
      nowPlayingTime.current_sec = reader->readElementText().toInt();
      lg->debug( QString( "NowPlayingObject::parseTime: current play time: %1" ).arg( nowPlayingTime.current_sec ) );
      lg->debug( "NowPlayingObject::parseTime: finished" );
    }
  }

  void NowPlayingObject::parseArt( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "art" ) );
    //
    // Logo(Icon o.A für Sender/Playlist
    //
    lg->debug( "NowPlayingObject::parseArt..." );
    if ( reader->name() == QLatin1String( "art" ) )
    {
      //
      // Attribut "artImageStatus"
      //
      lg->debug( "NowPlayingObject::parseArt: check for attribute \"artImageStatus\"..." );
      QXmlStreamAttributes attr = reader->attributes();
      if ( attr.hasAttribute( QLatin1String( "artImageStatus" ) ) )
      {
        nowPlayingArt.artImageStatus = attr.value( QLatin1String( "artImageStatus" ) ).toString();
        lg->debug(
            QString( "NowPlayingObject::parseArt: attribute \"artImageStatus\" has value %1" ).arg( nowPlayingArt.artImageStatus ) );
      }
      else
      {
        lg->warn( "NowPlayingObject::parseArt: there is no attribute \"source\"..." );
      }
      //
      // jetzt den Inhalt
      //
      nowPlayingArt.artUrl = reader->readElementText();
      lg->debug( QString( "NowPlayingObject::parseArt: artUrl: %1" ).arg( nowPlayingArt.artUrl ) );
      lg->debug( "NowPlayingObject::parseArt: finished" );
    }
  }

  void NowPlayingObject::parseContentItem( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "ContentItem" ) );
    //
    // source/location/sourceAccount/isPresetable finden (Attribute von <ContentItem>)
    //
    lg->debug( "NowPlayingObject::parseContentItem..." );
    if ( reader->name() == QLatin1String( "ContentItem" ) )
    {
      //
      // Attribut "source"
      //
      lg->debug( "NowPlayingObject::parseContentItem: check for attribute \"source\"..." );
      QXmlStreamAttributes attr = reader->attributes();
      if ( attr.hasAttribute( QLatin1String( "source" ) ) )
      {
        contentItem.source = attr.value( QLatin1String( "source" ) ).toString();
        lg->debug( QString( "NowPlayingObject::parseContentItem: attribute \"source\" has value %1" ).arg( contentItem.source ) );
      }
      else
      {
        lg->warn( "NowPlayingObject::parseContentItem: there is no attribute \"source\"..." );
      }
      //
      // Attribut "location"
      //
      lg->debug( "NowPlayingObject::parseContentItem: check for attribute \"location\"..." );
      if ( attr.hasAttribute( QLatin1String( "location" ) ) )
      {
        contentItem.location = attr.value( QLatin1String( "location" ) ).toString();
        lg->debug( QString( "NowPlayingObject::parseContentItem: attribute \"location\" has value %1" ).arg( contentItem.location ) );
      }
      else
      {
        lg->warn( "NowPlayingObject::parseContentItem: there is no attribute \"location\"..." );
      }
      //
      // Attribut "sourceAccount"
      //
      lg->debug( "NowPlayingObject::parseContentItem: check for attribute \"sourceAccount\"..." );
      if ( attr.hasAttribute( QLatin1String( "sourceAccount" ) ) )
      {
        contentItem.sourceAccount = attr.value( QLatin1String( "sourceAccount" ) ).toString();
        lg->debug( QString( "NowPlayingObject::parseContentItem: attribute \"sourceAccount\" has value %1" )
                       .arg( contentItem.sourceAccount ) );
      }
      else
      {
        lg->warn( "NowPlayingObject::parseContentItem: there is no attribute \"sourceAccount\"..." );
      }
      //
      // Attribut "isPresetable"
      //
      lg->debug( "NowPlayingObject::parseContentItem: check for attribute \"isPresetable\"..." );
      if ( attr.hasAttribute( "isPresetable" ) )
      {
        if ( attr.value( QLatin1String( "isPresetable" ) ) == "true" )
          contentItem.isPresetable = true;
        else
          contentItem.isPresetable = false;
        lg->debug(
            QString( "NowPlayingObject::parseContentItem: attribute \"isPresetable\" has value %1" ).arg( contentItem.isPresetable ) );
      }
      else
      {
        lg->warn( "NowPlayingObject::parseContentItem: there is no attribute \"isPresetable\"..." );
      }
    }
    //
    // jetzt die Elemente
    //
    while ( reader->readNextStartElement() )
    {
      //
      // welchen Eintrag hab ich gefunden?
      //
      if ( reader->name() == QLatin1String( "itemName" ) )
      {
        contentItem.itemName = reader->readElementText();
        lg->debug( QString( "NowPlayingObject::parseContentItem: itemName: %1" ).arg( contentItem.itemName ) );
      }
      else if ( reader->name() == QLatin1String( "containerArt" ) )
      {
        contentItem.containerArt = reader->readElementText();
        lg->debug( QString( "NowPlayingObject::parseContentItem: containerArt: %1" ).arg( contentItem.containerArt ) );
      }
      else
      {
        lg->warn( QString( "NowPlayingObject::parseContentItem: unsupportet entry %1 -> %2 " )
                      .arg( reader->name().toString() )
                      .arg( reader->readElementText() ) );
      }
    }
    lg->debug( "NowPlayingObject::parseContentItem: finished." );
  }
}  // namespace radio
