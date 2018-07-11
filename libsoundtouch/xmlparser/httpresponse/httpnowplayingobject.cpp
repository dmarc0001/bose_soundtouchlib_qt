#include "httpnowplayingobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpNowPlayingObject::HttpNowPlayingObject
   * @param xmlreader
   * @param parent
   */
  HttpNowPlayingObject::HttpNowPlayingObject( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() &&
              ( reader->name() == QLatin1String( "nowPlaying" ) || reader->name() == QLatin1String( "nowPlayingUpdated" ) ) );
    //
    if ( reader->name() == QLatin1String( "nowPlaying" ) )
    {
      resultType = ResultobjectType::R_NOW_PLAYING;
    }
    else
    {
      // if ( reader -> readNextStartElement() && !reader->hasError() )
      while ( IResponseObject::getNextStartTag( reader ) )
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
    qDebug() << "...";
    deviceId = IResponseObject::getAttribute( reader, QLatin1String( "deviceID" ) );
    qDebug() << "device id: " << deviceId;
    source = IResponseObject::getAttribute( reader, QLatin1String( "source" ) );
    qDebug() << "playing source: " << source;
    sourceAccount = IResponseObject::getAttribute( reader, QLatin1String( "sourceAccount" ) );
    if ( !sourceAccount.isEmpty() )
      qDebug() << "source Account: " << sourceAccount;
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      if ( reader->name() == QLatin1String( "ContentItem" ) )
      {
        //
        // Objet mit Attributen (kannman bei /select nutzen)
        //
        contentItem = std::unique_ptr< ContentItem >( new ContentItem( reader, this ) );
        qDebug() << "ContentItem.source:" << contentItem->source;
        qDebug() << "ContentItem.type:" << contentItem->type;
        qDebug() << "ContentItem.location:" << contentItem->location;
        qDebug() << "ContentItem.sourceAccount:" << contentItem->sourceAccount;
        qDebug() << "ContentItem.itemName:" << contentItem->itemName;
        qDebug() << "ContentItem.containerArt:" << contentItem->containerArt;
      }
      else if ( reader->name() == QLatin1String( "track" ) )
      {
        //
        // gespielter track
        //
        track = reader->readElementText();
        qDebug() << "track: " << track;
      }
      else if ( reader->name() == QLatin1String( "artist" ) )
      {
        //
        // gespielter künstler
        //
        artist = reader->readElementText();
        qDebug() << "artist: " << artist;
      }
      else if ( reader->name() == QLatin1String( "album" ) )
      {
        //
        // gespieltes album
        //
        album = reader->readElementText();
        qDebug() << "album: " << album;
      }
      else if ( reader->name() == QLatin1String( "genre" ) )
      {
        //
        // gespieltes genre
        //
        genre = reader->readElementText();
        qDebug() << "genre: " << genre;
      }
      else if ( reader->name() == QLatin1String( "stationName" ) )
      {
        //
        // gespielte station/playlist
        //
        stationName = reader->readElementText();
        qDebug() << "station or playlist: " << stationName;
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
        qDebug() << "skip enabled.";
      }
      else if ( reader->name() == QLatin1String( "skipPreviousEnabled" ) )
      {
        //
        // zurück springen möglich?
        //
        skipPreviousEnabled = true;
        qDebug() << "skip previous enabled.";
      }
      else if ( reader->name() == QLatin1String( "skipPreviousSupported" ) )
      {
        //
        // zurück springen unterstützt?
        //
        if ( IResponseObject::getAttribute( reader, QLatin1String( "value" ) ) == QLatin1String( "true" ) )
        {
          skipPreviousSupported = true;
        }
        qDebug() << "skip previous supported: " << skipPreviousSupported;
      }
      else if ( reader->name() == QLatin1String( "favoriteEnabled" ) )
      {
        //
        // als favorite möglich?
        //
        favoriteEnabled = true;
        qDebug() << "favoriteEnabled enabled.";
      }
      else if ( reader->name() == QLatin1String( "isFavorite" ) )
      {
        //
        // als favorite möglich?
        //
        isFavorite = true;
        qDebug() << "isFavorite enabled.";
      }
      else if ( reader->name() == QLatin1String( "rateEnabled" ) )
      {
        //
        // bewertung möglich?
        //
        rateEnabled = true;
        qDebug() << "rateEnabled enabled.";
      }
      else if ( reader->name() == QLatin1String( "rating" ) )
      {
        //
        // Bewertng (zweideutig in der Doku)
        //
        rating = reader->readElementText();
        qDebug() << "rating: " << rating;
      }
      else if ( reader->name() == QLatin1String( "playStatus" ) )
      {
        //
        // welcher Status beim abspielen
        //
        playStatus = reader->readElementText();
        qDebug() << "playStatus: " << playStatus;
      }
      else if ( reader->name() == QLatin1String( "shuffleSettings" ) )
      {
        //
        // zufallabspielen einstellungen
        //
        shuffleSettings = reader->readElementText();
        qDebug() << "shuffleSettings: " << shuffleSettings;
      }
      else if ( reader->name() == QLatin1String( "repeatSettings" ) )
      {
        //
        // Wiederholungseinstellungen
        //
        repeatSettings = reader->readElementText();
        qDebug() << "repeatSettings: %1" << repeatSettings;
      }
      else if ( reader->name() == QLatin1String( "streamType" ) )
      {
        //
        // Art des Streams
        //
        streamType = reader->readElementText();
        qDebug() << "streamType: " << streamType;
      }
      else if ( reader->name() == QLatin1String( "description" ) )
      {
        //
        // Beschreibung des Streams
        //
        _description = reader->readElementText();
        qDebug() << "stream description: " << _description;
      }
      else if ( reader->name() == QLatin1String( "stationLocation" ) )
      {
        //
        // station standort (internet only)
        //
        stationLocation = reader->readElementText();
        qDebug() << "station location: " << stationLocation;
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
        qWarning() << "unsupported tag: " << reader->name().toString() << " --> " << reader->readElementText();
      }
    }
  }

  /**
   * @brief HttpNowPlayingObject::~HttpNowPlayingObject
   */
  HttpNowPlayingObject::~HttpNowPlayingObject()
  {
    qDebug() << "...";
  }

  /**
   * @brief HttpNowPlayingObject::parseConnectionStatusInfo
   */
  void HttpNowPlayingObject::parseConnectionStatusInfo( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "time" ) );
    //
    // Spielzeit
    //
    qDebug() << "...";
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      //
      // welchen Eintrag hab ich gefunden?
      //
      if ( reader->name() == QLatin1String( "status" ) )
      {
        nowPlayingConnectStatusInfo.status = reader->readElementText();
        qDebug() << "status: " << nowPlayingConnectStatusInfo.status;
      }
      else if ( reader->name() == QLatin1String( "deviceName" ) )
      {
        nowPlayingConnectStatusInfo.deviceName = reader->readElementText();
        qDebug() << "device name: " << nowPlayingConnectStatusInfo.deviceName;
      }
      else
      {
        qWarning() << "unsupported tag: " << reader->name().toString() << " --> " << reader->readElementText();
      }
    }
  }

  /**
   * @brief HttpNowPlayingObject::parseTime
   */
  void HttpNowPlayingObject::parseTime( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "time" ) );
    //
    // Spielzeit
    //
    qDebug() << "...";
    //
    // Attribut "total"
    //
    nowPlayingTime.total_sec = IResponseObject::getAttribute( reader, QLatin1String( "total" ) ).toInt();
    //
    // jetzt den Inhalt
    //
    nowPlayingTime.current_sec = reader->readElementText().toInt();
    qDebug() << "current play time: " << nowPlayingTime.current_sec;
  }

  /**
   * @brief HttpNowPlayingObject::parseArt
   */
  void HttpNowPlayingObject::parseArt( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "art" ) );
    //
    // Logo(Icon o.A für Sender/Playlist
    //
    qDebug() << "...";
    //
    // Attribut "artImageStatus"
    //
    nowPlayingArt.artImageStatus = IResponseObject::getAttribute( reader, QLatin1String( "artImageStatus" ) ).toInt();
    //
    // jetzt den Inhalt
    //
    nowPlayingArt.artUrl = reader->readElementText();
    qDebug() << "artUrl: " << nowPlayingArt.artUrl;
  }

  //
  // GETTER
  //

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

}  // namespace bose_soundtoch_lib
