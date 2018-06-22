#ifndef NOWPLAYINGOBJECT_HPP
#define NOWPLAYINGOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include "iresponseobject.hpp"

namespace radio
{
  class NowPlayingContentItem
  {
    public:
    QString source;
    QString location;
    QString sourceAccount;
    bool isPresetable;
    QString itemName;
    QString containerArt;
  };

  class NowPlayingArt
  {
    public:
    QString artImageStatus;
    QString artUrl;
  };

  class NowPlayingTime
  {
    public:
    int total_sec;
    int current_sec;
  };

  class NowPlayingConnectionStatusInfo
  {
    public:
    QString status;
    QString deviceName;
  };

  class NowPlayingObject : public IResponseObject
  {
    private:
    QString deviceId;
    QString source;
    NowPlayingContentItem contentItem;
    QString track;
    QString artist;
    QString album;
    QString genre;
    QString ratingUser;  //! zweideutig "rating" user rating of the song
    QString stationName;
    NowPlayingArt nowPlayingArt;
    NowPlayingTime nowPlayingTime;
    bool skipEnabled = false;
    bool skipPreviousEnabled = false;
    bool skipPreviousSupported = false;
    bool favoriteEnabled = false;
    bool isFavorite = false;
    bool rateEnabled = false;
    QString rating;  //! UP,DOWN,NONE
    QString playStatus;
    QString shuffleSettings;
    QString repeatSettings;
    QString streamType;
    QString _description;     //! unsupported
    QString stationLocation;  //! Internet only
    NowPlayingConnectionStatusInfo nowPlayingConnectStatusInfo;

    public:
    explicit NowPlayingObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent = nullptr );
    ~NowPlayingObject() override;
    void parseContentItem( void );

    private:
    void parseArt( void );
    void parseTime( void );
    void parseConnectionStatusInfo( void );
  };
}  // namespace radio
#endif  // NOWPLAYINGOBJECT_HPP
