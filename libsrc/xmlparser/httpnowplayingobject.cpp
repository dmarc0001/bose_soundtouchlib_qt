#include "httpnowplayingobject.hpp"

namespace bose_soundtoch_lib
{
  HttpNowPlayingObject::HttpNowPlayingObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() &&
              ( reader->name() == QLatin1String( "nowPlaying" ) || reader->name() == QLatin1String( "nowPlayingUpdated" ) ) );
    if ( reader->name() == QLatin1String( "nowPlaying" ) )
    {
      resultType = ResultobjectType::R_NOW_PLAYING;
    }
    else
    {
      if ( reader->readNextStartElement() && !reader->hasError() )
      {
        if ( reader->name() == QLatin1String( "nowPlaying" ) )
        {
          resultType = ResultobjectType::U_NOWPLAYING;
        }
        else
        {
          resultType = ResultobjectType::R_UNKNOWN;
          return;
        }
      }
    }
    //
    // DeviceID/source finden (Attribute von <nowPlaying>)
    //
    lg->debug( "NowPlayingObject::NowPlayingObject..." );
    lg->debug( "NowPlayingObject::NowPlayingObject: check for attribute in \"nowPlaying\"..." );
    deviceId = getAttibute( reader, QLatin1String( "deviceID" ) );
    source = getAttibute( reader, QLatin1String( "source" ) );
    sourceAccount = getAttibute( reader, QLatin1String( "sourceAccount" ) );
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
        contentItem = std::unique_ptr< ContentItem >( new ContentItem( lg, reader, this ) );
        // parseContentItem();
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
        skipPreviousEnabled = true;
        lg->debug( "NowPlayingObject::NowPlayingObject: skip previous enabled." );
      }
      else if ( reader->name() == QLatin1String( "skipPreviousSupported" ) )
      {
        //
        // zurück springen unterstützt?
        //
        if ( getAttibute( reader, QLatin1String( "value" ) ) == QLatin1String( "true" ) )
        {
          skipPreviousSupported = true;
        }
        lg->debug( QString( "NowPlayingObject::NowPlayingObject: skip previous supported:" ).arg( skipPreviousSupported ) );
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

  HttpNowPlayingObject::~HttpNowPlayingObject()
  {
    lg->debug( "NowPlayingObject::~NowPlayingObject..." );
  }

  void HttpNowPlayingObject::parseConnectionStatusInfo( void )
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

  void HttpNowPlayingObject::parseTime( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "time" ) );
    //
    // Spielzeit
    //
    lg->debug( "NowPlayingObject::parseTime..." );
    //
    // Attribut "total"
    //
    lg->debug( "NowPlayingObject::parseTime: check for attribute in \"time\"..." );
    nowPlayingTime.total_sec = getAttibute( reader, QLatin1String( "total" ) ).toInt();
    //
    // jetzt den Inhalt
    //
    nowPlayingTime.current_sec = reader->readElementText().toInt();
    lg->debug( QString( "NowPlayingObject::parseTime: current play time: %1" ).arg( nowPlayingTime.current_sec ) );
    lg->debug( "NowPlayingObject::parseTime: finished" );
  }

  void HttpNowPlayingObject::parseArt( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "art" ) );
    //
    // Logo(Icon o.A für Sender/Playlist
    //
    lg->debug( "NowPlayingObject::parseArt..." );
    //
    // Attribut "artImageStatus"
    //
    lg->debug( "NowPlayingObject::parseArt: check for attribute in \"art\"..." );
    nowPlayingArt.artImageStatus = getAttibute( reader, QLatin1String( "artImageStatus" ) ).toInt();
    //
    // jetzt den Inhalt
    //
    nowPlayingArt.artUrl = reader->readElementText();
    lg->debug( QString( "NowPlayingObject::parseArt: artUrl: %1" ).arg( nowPlayingArt.artUrl ) );
    lg->debug( "NowPlayingObject::parseArt: finished" );
  }

  QString HttpNowPlayingObject::getSource() const
  {
    return source;
  }

  std::shared_ptr< ContentItem > HttpNowPlayingObject::getContentItem() const
  {
    return ( contentItem );
  }

  QString HttpNowPlayingObject::getTrack() const
  {
    return track;
  }

  QString HttpNowPlayingObject::getArtist() const
  {
    return artist;
  }

  QString HttpNowPlayingObject::getAlbum() const
  {
    return album;
  }

  QString HttpNowPlayingObject::getGenre() const
  {
    return genre;
  }

  QString HttpNowPlayingObject::getRatingUser() const
  {
    return ratingUser;
  }

  QString HttpNowPlayingObject::getStationName() const
  {
    return stationName;
  }

  PlayingArt HttpNowPlayingObject::getNowPlayingArt() const
  {
    return nowPlayingArt;
  }

  PlayingTime HttpNowPlayingObject::getNowPlayingTime() const
  {
    return nowPlayingTime;
  }

  bool HttpNowPlayingObject::getSkipEnabled() const
  {
    return skipEnabled;
  }

  bool HttpNowPlayingObject::getSkipPreviousEnabled() const
  {
    return skipPreviousEnabled;
  }

  bool HttpNowPlayingObject::getSkipPreviousSupported() const
  {
    return skipPreviousSupported;
  }

  bool HttpNowPlayingObject::getFavoriteEnabled() const
  {
    return favoriteEnabled;
  }

  bool HttpNowPlayingObject::getIsFavorite() const
  {
    return isFavorite;
  }

  bool HttpNowPlayingObject::getRateEnabled() const
  {
    return rateEnabled;
  }

  QString HttpNowPlayingObject::getRating() const
  {
    return rating;
  }

  QString HttpNowPlayingObject::getPlayStatus() const
  {
    return playStatus;
  }

  QString HttpNowPlayingObject::getShuffleSettings() const
  {
    return shuffleSettings;
  }

  QString HttpNowPlayingObject::getRepeatSettings() const
  {
    return repeatSettings;
  }

  QString HttpNowPlayingObject::getStreamType() const
  {
    return streamType;
  }

  QString HttpNowPlayingObject::getDescription() const
  {
    return _description;
  }

  QString HttpNowPlayingObject::getStationLocation() const
  {
    return stationLocation;
  }

  DeviceConnectionStatusInfo HttpNowPlayingObject::getNowPlayingConnectStatusInfo() const
  {
    return nowPlayingConnectStatusInfo;
  }

  QString HttpNowPlayingObject::getSourceAccount() const
  {
    return sourceAccount;
  }

}  // namespace radio
