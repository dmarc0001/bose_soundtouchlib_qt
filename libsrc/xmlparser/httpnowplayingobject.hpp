#ifndef NOWPLAYINGOBJECT_HPP
#define NOWPLAYINGOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QtDebug>
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class HttpNowPlayingObject : public IResponseObject
  {
    private:
    QString source;                                          //! Quelle die gespielt wird (internet, Bluetooth etc...)
    QString sourceAccount;                                   //! account zur Quelle
    std::shared_ptr< ContentItem > contentItem;              //! Beschreibung der Quelle
    QString track;                                           //! Titel aktuell
    QString artist;                                          //! Künstler aktuell
    QString album;                                           //! Album aktuell
    QString genre;                                           //! Genre aktuell
    QString ratingUser;                                      //! zweideutig "rating" user rating of the song
    QString stationName;                                     //! Sender oder Playlist
    PlayingArt nowPlayingArt;                                //! Logo für aktuell
    PlayingTime nowPlayingTime;                              //! Aktuelle Spielzeit
    bool skipEnabled = false;                                //! Überspringen erlaubt?
    bool skipPreviousEnabled = false;                        //! Zurückspringen erlaubt?
    bool skipPreviousSupported = false;                      //! zurückspringen unterstützt?
    bool favoriteEnabled = false;                            //! als Faforiten setzen erlaubt?
    bool isFavorite = false;                                 //! ist das ein Fafiorit?
    bool rateEnabled = false;                                //! Abstimmung möglich
    QString rating;                                          //! UP,DOWN,NONE
    QString playStatus;                                      //! Abspielsstatus
    QString shuffleSettings;                                 //! Zufallseinstellungen
    QString repeatSettings;                                  //! Wiederholung (aus, titel, alles)
    QString streamType;                                      //! Type des Mediums
    QString _description;                                    //! unsupported Beschreibung
    QString stationLocation;                                 //! Internet only
    DeviceConnectionStatusInfo nowPlayingConnectStatusInfo;  //! Berbindungsstatus als Objekt

    public:
    explicit HttpNowPlayingObject( QXmlStreamReader *xmlreader, QObject *parent = nullptr );
    ~HttpNowPlayingObject() override;
    //
    // GETTER
    //
    QString getSource() const;
    std::shared_ptr< ContentItem > getContentItem() const;
    QString getTrack() const;
    QString getArtist() const;
    QString getAlbum() const;
    QString getGenre() const;
    QString getRatingUser() const;
    QString getStationName() const;
    PlayingArt getNowPlayingArt() const;
    PlayingTime getNowPlayingTime() const;
    bool getSkipEnabled() const;
    bool getSkipPreviousEnabled() const;
    bool getSkipPreviousSupported() const;
    bool getFavoriteEnabled() const;
    bool getIsFavorite() const;
    bool getRateEnabled() const;
    QString getRating() const;
    QString getPlayStatus() const;
    QString getShuffleSettings() const;
    QString getRepeatSettings() const;
    QString getStreamType() const;
    QString getDescription() const;
    QString getStationLocation() const;
    DeviceConnectionStatusInfo getNowPlayingConnectStatusInfo() const;
    QString getSourceAccount() const;

    private:
    void parseArt( void );                   //! Parse Logobescheibung
    void parseTime( void );                  //! Parse die zeiten(aktuell, total)
    void parseConnectionStatusInfo( void );  //! parse das Objekt für Verbindungsstatus
  };
}  // namespace bose_soundtoch_lib
#endif  // NOWPLAYINGOBJECT_HPP
