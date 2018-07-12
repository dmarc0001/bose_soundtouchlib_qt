#include "httpnowplayingobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpNowPlayingObject::HttpNowPlayingObject
   * @param xmlreader
   * @param parent
   */
  HttpNowPlayingObject::HttpNowPlayingObject( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "nowPlaying" ) || domElem->tagName() == QLatin1String( "nowPlayingUpdated" ) );
    QDomNodeList rootChildNodesList;
    //
    if ( domElem->tagName() == QLatin1String( "nowPlayingUpdated" ) )
    {
      resultType = ResultobjectType::U_NOWPLAYING;
      rootChildNodesList = domElem->firstChild().childNodes();
      QDomNode rnode( domElem->firstChild() );
      deviceId = IResponseObject::getAttribute( &rnode, QLatin1String( "deviceID" ) );
      source = IResponseObject::getAttribute( &rnode, QLatin1String( "source" ) );
      sourceAccount = IResponseObject::getAttribute( &rnode, QLatin1String( "sourceAccount" ) );
    }
    else
    {
      if ( domElem->tagName() == QLatin1String( "nowPlaying" ) )
      {
        resultType = ResultobjectType::R_NOW_PLAYING;
        rootChildNodesList = domElem->childNodes();
        deviceId = IResponseObject::getAttribute( domElem, QLatin1String( "deviceID" ) );
        source = IResponseObject::getAttribute( domElem, QLatin1String( "source" ) );
        sourceAccount = IResponseObject::getAttribute( domElem, QLatin1String( "sourceAccount" ) );
      }
      else
      {
        resultType = ResultobjectType::R_UNKNOWN;
        return;
      }
    }
    //
    // DeviceID/source finden (Attribute von <nowPlaying>)
    //
    qDebug() << "...";
    qDebug() << "device id: " << deviceId;
    qDebug() << "playing source: " << source;
    if ( !sourceAccount.isEmpty() )
      qDebug() << "source Account: " << sourceAccount;
    //
    // lese soweit neue Elemente vorhanden sind
    //
    for ( int nodeIdx = 0; nodeIdx < rootChildNodesList.length(); nodeIdx++ )
    {
      QDomNode currNode( rootChildNodesList.item( nodeIdx ) );
      if ( currNode.isNull() )
        continue;
      //
      // unterscheide die Knoten
      // der Name ist hier als QString
      //
      QString currName( currNode.nodeName() );
      //
      if ( currName.compare( QLatin1String( "ContentItem" ), Qt::CaseInsensitive ) == 0 )
      {
        //
        // Objet mit Attributen (kannman bei /select nutzen)
        //
        contentItem = std::unique_ptr< ContentItem >( new ContentItem( &currNode, this ) );
        qDebug() << "ContentItem.source:" << contentItem->source;
        if ( contentItem->source == QLatin1String( "STANDBY" ) )
        {
          continue;
        }
        else
        {
          qDebug() << "ContentItem.type:" << contentItem->type;
          qDebug() << "ContentItem.location:" << contentItem->location;
          qDebug() << "ContentItem.sourceAccount:" << contentItem->sourceAccount;
          qDebug() << "ContentItem.itemName:" << contentItem->itemName;
          qDebug() << "ContentItem.containerArt:" << contentItem->containerArt;
        }
      }
      else if ( currName == QLatin1String( "track" ) )
      {
        //
        // gespielter track
        //
        track = currNode.toElement().text();
        qDebug() << "track: " << track;
      }
      else if ( currName == QLatin1String( "artist" ) )
      {
        //
        // gespielter künstler
        //
        artist = currNode.toElement().text();
        qDebug() << "artist: " << artist;
      }
      else if ( currName == QLatin1String( "album" ) )
      {
        //
        // gespieltes album
        //
        album = currNode.toElement().text();
        qDebug() << "album: " << album;
      }
      else if ( currName == QLatin1String( "genre" ) )
      {
        //
        // gespieltes genre
        //
        genre = currNode.toElement().text();
        qDebug() << "genre: " << genre;
      }
      else if ( currName == QLatin1String( "stationName" ) )
      {
        //
        // gespielte station/playlist
        //
        stationName = currNode.toElement().text();
        qDebug() << "station or playlist: " << stationName;
      }
      else if ( currName == QLatin1String( "art" ) )
      {
        //
        // Dekoration für Sender/Playlist (Logo etc)
        //
        parseArt( &currNode );
      }
      else if ( currName == QLatin1String( "time" ) )
      {
        //
        // Laufzeit
        //
        parseTime( &currNode );
      }
      else if ( currName == QLatin1String( "skipEnabled" ) )
      {
        //
        // weiter springen möglich?
        //
        skipEnabled = true;
        qDebug() << "skip enabled.";
      }
      else if ( currName == QLatin1String( "skipPreviousEnabled" ) )
      {
        //
        // zurück springen möglich?
        //
        skipPreviousEnabled = true;
        qDebug() << "skip previous enabled.";
      }
      else if ( currName == QLatin1String( "skipPreviousSupported" ) )
      {
        //
        // zurück springen unterstützt?
        //
        if ( IResponseObject::getAttribute( &currNode, QLatin1String( "value" ) ) == QLatin1String( "true" ) )
        {
          skipPreviousSupported = true;
        }
        qDebug() << "skip previous supported: " << skipPreviousSupported;
      }
      else if ( currName == QLatin1String( "favoriteEnabled" ) )
      {
        //
        // als favorite möglich?
        //
        favoriteEnabled = true;
        qDebug() << "favoriteEnabled enabled.";
      }
      else if ( currName == QLatin1String( "isFavorite" ) )
      {
        //
        // als favorite möglich?
        //
        isFavorite = true;
        qDebug() << "isFavorite enabled.";
      }
      else if ( currName == QLatin1String( "rateEnabled" ) )
      {
        //
        // bewertung möglich?
        //
        rateEnabled = true;
        qDebug() << "rateEnabled enabled.";
      }
      else if ( currName == QLatin1String( "rating" ) )
      {
        //
        // Bewertng (zweideutig in der Doku)
        //
        rating = currNode.toElement().text();
        qDebug() << "rating: " << rating;
      }
      else if ( currName == QLatin1String( "playStatus" ) )
      {
        //
        // welcher Status beim abspielen
        //
        playStatus = currNode.toElement().text();
        qDebug() << "playStatus: " << playStatus;
      }
      else if ( currName == QLatin1String( "shuffleSettings" ) )
      {
        //
        // zufallabspielen einstellungen
        //
        shuffleSettings = currNode.toElement().text();
        qDebug() << "shuffleSettings: " << shuffleSettings;
      }
      else if ( currName == QLatin1String( "repeatSettings" ) )
      {
        //
        // Wiederholungseinstellungen
        //
        repeatSettings = currNode.toElement().text();
        qDebug() << "repeatSettings: %1" << repeatSettings;
      }
      else if ( currName == QLatin1String( "streamType" ) )
      {
        //
        // Art des Streams
        //
        streamType = currNode.toElement().text();
        qDebug() << "streamType: " << streamType;
      }
      else if ( currName == QLatin1String( "description" ) )
      {
        //
        // Beschreibung des Streams
        //
        _description = currNode.toElement().text();
        qDebug() << "stream description: " << _description;
      }
      else if ( currName == QLatin1String( "stationLocation" ) )
      {
        //
        // station standort (internet only)
        //
        stationLocation = currNode.toElement().text();
        ;
        qDebug() << "station location: " << stationLocation;
      }
      else if ( currName == QLatin1String( "connectionStatusInfo" ) )
      {
        parseConnectionStatusInfo( &currNode );
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag: " << currName << " --> " << currNode.toElement().text();
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
  void HttpNowPlayingObject::parseConnectionStatusInfo( QDomNode *node )
  {
    Q_ASSERT( node->nodeName() == QLatin1String( "connectionStatusInfo" ) );
    //
    // Spielzeit
    //
    qDebug() << "...";
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    QDomNodeList childNodesList( node->childNodes() );
    for ( int nodeIdx = 0; nodeIdx < childNodesList.length(); nodeIdx++ )
    {
      QDomNode currNode( childNodesList.item( nodeIdx ) );
      if ( currNode.isNull() )
        continue;
      if ( currNode.nodeName() == QLatin1String( "status" ) )
      {
        nowPlayingConnectStatusInfo.status = currNode.toElement().text();
        qDebug() << "status: " << nowPlayingConnectStatusInfo.status;
      }
      else if ( currNode.nodeName() == QLatin1String( "deviceName" ) )
      {
        nowPlayingConnectStatusInfo.deviceName = currNode.toElement().text();
        qDebug() << "device name: " << nowPlayingConnectStatusInfo.deviceName;
      }
      else
      {
        qWarning() << "unsupported tag: " << currNode.nodeName() << " --> " << currNode.toElement().text();
      }
    }
  }

  /**
   * @brief HttpNowPlayingObject::parseTime
   */
  void HttpNowPlayingObject::parseTime( QDomNode *node )
  {
    Q_ASSERT( node->nodeName() == QLatin1String( "time" ) );
    //
    // Spielzeit
    //
    qDebug() << "...";
    //
    // Attribut "total"
    //
    nowPlayingTime.total_sec = IResponseObject::getAttribute( node, QLatin1String( "total" ) ).toInt();
    //
    // jetzt den Inhalt
    //
    nowPlayingTime.current_sec = node->toElement().text().toInt();
    qDebug() << "current play time: " << nowPlayingTime.current_sec;
  }

  /**
   * @brief HttpNowPlayingObject::parseArt
   */
  void HttpNowPlayingObject::parseArt( QDomNode *node )
  {
    Q_ASSERT( node->nodeName() == QLatin1String( "art" ) );
    //
    // Logo(Icon o.A für Sender/Playlist
    //
    qDebug() << "...";
    //
    // Attribut "artImageStatus"
    //
    nowPlayingArt.artImageStatus = IResponseObject::getAttribute( node, QLatin1String( "artImageStatus" ) ).toInt();
    //
    // jetzt den Inhalt
    //
    nowPlayingArt.artUrl = node->toElement().text();
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
