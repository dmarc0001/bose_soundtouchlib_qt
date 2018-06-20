#include "bsoundtouchdevice.hpp"
#include <QThread>

namespace radio
{
  //! Strings für /key festlegen
  const char *BSoundTouchDevice::keynames[] = {
      "PLAY",       "PAUSE",      "PLAYPAUSE",  "PREV_TRACK",   "NEXT_TRACK",      "MUTE",      "SHUFFLE_ON",  "SHUFFLE_OFF",
      "REPEAT_ONE", "REPEAT_ALL", "REPEAT_OFF", "ADD_FAVORITE", "REMOVE_FAVORITE", "THUMBS_UP", "THUMBS_DOWN", "BOOKMARK",
      "PRESET_1",   "PRESET_2",   "PRESET_3",   "PRESET_4",     "PRESET_5",        "PRESET_6",  "POWER",       "UNKNOWN"};
  //! Strings für /key keystati
  const char *BSoundTouchDevice::keystati[] = {"press", "release", "toggle"};
  //! Subprotokoll für soundtouch, ist imer Gabbo
  const char *BSoundTouchDevice::subproto = "Gabbo";
  /**
   * @brief BSoundTouchDevice::BSoundTouchDevice
   * @param stHost
   * @param stHttpPort
   * @param stWSPort
   * @param logger
   * @param parent
   */
  BSoundTouchDevice::BSoundTouchDevice( QString &stHost,
                                        qint16 stHttpPort,
                                        qint16 stWSPort,
                                        std::shared_ptr< Logger > logger,
                                        QObject *parent )
      : QObject( parent ), hostname( stHost ), httpPort( stHttpPort ), wsPort( stWSPort ), lg( logger )
  {
    lg->debug( "BSoundTouchDevice::BSoundTouchDevice..." );
    connect( &qnam, &QNetworkAccessManager::authenticationRequired, this, &BSoundTouchDevice::slotAuthenticationRequired );
  }

  /**
   * @brief BSoundTouchDevice::~BSoundTouchDevice
   */
  BSoundTouchDevice::~BSoundTouchDevice()
  {
    lg->debug( "BSoundTouchDevice::~BSoundTouchDevice..." );
  }

  void BSoundTouchDevice::setHostname( QString &stHost )
  {
    hostname = stHost;
  }

  void BSoundTouchDevice::setHttpPort( qint16 stHttpPort )
  {
    httpPort = stHttpPort;
  }

  void BSoundTouchDevice::setWSPort( qint16 stWSPort )
  {
    wsPort = stWSPort;
  }

  //###########################################################################
  //#### Informationsanfragen HTTP-GET                                     ####
  //###########################################################################

  void BSoundTouchDevice::getVolume( void )
  {
    setUrl( "volume" );
    startGetRequest();
  }

  void BSoundTouchDevice::getSources( void )
  {
    setUrl( "sources" );
    startGetRequest();
  }

  void BSoundTouchDevice::getBassCapabilities( void )
  {
    setUrl( "bassCapabilities" );
    startGetRequest();
  }

  void BSoundTouchDevice::getBass( void )
  {
    setUrl( "bass" );
    startGetRequest();
  }

  void BSoundTouchDevice::getZone( void )
  {
    setUrl( "getZone" );
    startGetRequest();
  }

  void BSoundTouchDevice::getNowPlaying( void )
  {
    setUrl( "now_playing" );
    startGetRequest();
  }

  void BSoundTouchDevice::getTrackInfo( void )
  {
    setUrl( "trackinfo" );
    startGetRequest();
  }

  void BSoundTouchDevice::getPresets( void )
  {
    setUrl( "presets" );
    startGetRequest();
  }

  void BSoundTouchDevice::getDeviceInfo( void )
  {
    setUrl( "info" );
    startGetRequest();
  }

  void BSoundTouchDevice::startGetRequest( const QUrl &requested )
  {
    url = requested;
    startGetRequest();
  }

  void BSoundTouchDevice::startGetRequest( const QString &requested )
  {
    url = QUrl( QString( "http://%1:%2/%3" ).arg( hostname ).arg( httpPort ).arg( requested ) );
    startGetRequest( url );
  }

  void BSoundTouchDevice::startGetRequest( const char *requested )
  {
    url = QUrl( QString( "http://%1:%2/%3" ).arg( hostname ).arg( httpPort ).arg( requested ) );
    startGetRequest( url );
  }

  //###########################################################################
  //#### SET Funktionen (HTTP POST)                                        ####
  //###########################################################################

  void BSoundTouchDevice::setKey( bose_key whichkey, bose_keystate keystate, QString sender )
  {
    QString data;
    //
    lg->info( QString( "BSoundTouchDevice::setKey: %1 %2 from '%3' in %4" )
                  .arg( BSoundTouchDevice::keynames[ static_cast< int >( whichkey ) ] )
                  .arg( BSoundTouchDevice::keystati[ static_cast< int >( keystate ) ] )
                  .arg( sender )
                  .arg( hostname ) );
    //
    // die URL steht schon einmal fest
    //
    setUrl( "key" );
    //
    // welche Tasten, welche Reaktion
    // zuerst POWER
    //
    if ( whichkey == bose_key::KEY_POWER )
    {
      if ( keystate == bose_keystate::KEY_TOGGLE )
      {
        lg->info( "BSoundTouchDevice::setKey: power switch (toggle)..." );
        data = QString( "<key state=\"%1\" sender=\"%2\">%3</key>" )
                   .arg( keystati[ static_cast< int >( bose_keystate::KEY_PRESSED ) ] )
                   .arg( BSoundTouchDevice::subproto )
                   .arg( keynames[ static_cast< int >( bose_key::KEY_POWER ) ] );
        startPostRequest( data );
        QThread::msleep( 60 );
        data = QString( "<key state=\"%1\" sender=\"%2\">%3</key>" )
                   .arg( keystati[ static_cast< int >( bose_keystate::KEY_RELEASED ) ] )
                   .arg( BSoundTouchDevice::subproto )
                   .arg( keynames[ static_cast< int >( bose_key::KEY_POWER ) ] );
        startPostRequest( data );
        return;
      }
      else
      {
        lg->info( QString( "BSoundTouchDevice::setKey: power switch (%1)..." ).arg( keystati[ static_cast< int >( keystate ) ] ) );
        data = QString( "<key state=\"%1\" sender=\"%2\">%3</key>" )
                   .arg( keystati[ static_cast< int >( keystate ) ] )
                   .arg( BSoundTouchDevice::subproto )
                   .arg( keynames[ static_cast< int >( bose_key::KEY_POWER ) ] );
        startPostRequest( data );
        return;
      }
    }
    //
    // der REST
    //
    if ( keystate == bose_keystate::KEY_TOGGLE )
    {
      data = QString( "<key state =\"%1\" sender=\"%2\">%3</key>" )
                 .arg( keystati[ static_cast< int >( bose_keystate::KEY_PRESSED ) ] )
                 .arg( BSoundTouchDevice::subproto )
                 .arg( keynames[ static_cast< int >( whichkey ) ] );
      startPostRequest( data );
      QThread::msleep( 60 );
      data = QString( "<key state =\"%1\" sender=\"%2\">%3</key>" )
                 .arg( keystati[ static_cast< int >( bose_keystate::KEY_RELEASED ) ] )
                 .arg( BSoundTouchDevice::subproto )
                 .arg( keynames[ static_cast< int >( whichkey ) ] );
      startPostRequest( data );
    }
    else
    {
      data = QString( "<key state =\"%1\" sender=\"%2\">%3</key>" )
                 .arg( keystati[ static_cast< int >( keystate ) ] )
                 .arg( BSoundTouchDevice::subproto )
                 .arg( keynames[ static_cast< int >( whichkey ) ] );
      startPostRequest( data );
    }
  }

  //###########################################################################
  //#### hilfsfunktionen GET/POST                                          ####
  //###########################################################################

  void BSoundTouchDevice::startGetRequest( void )
  {
    lg->debug( QString( "BSoundTouchDevice::start GET Request: %1" ).arg( url.toString() ) );
    QNetworkReply *reply = qnam.get( QNetworkRequest( url ) );
    //
    // für Asynchrone Verarbeitung anbinden...
    //
    connect( reply, &QNetworkReply::finished, this, [=] { httpFinished( reply ); } );
    connect( reply, &QIODevice::readyRead, this, [=] { httpReadyRead( reply ); } );
  }

  void BSoundTouchDevice::setUrl( const char *path )
  {
    url = QUrl( QString( "http://%1:%2/%3" ).arg( hostname ).arg( httpPort ).arg( path ) );
  }

  void BSoundTouchDevice::startPostRequest( const char *requested, const QString &data )
  {
    setUrl( requested );
    startPostRequest( data );
  }

  void BSoundTouchDevice::startPostRequest( const QUrl &requested, const QString &data )
  {
    url = requested;
    startPostRequest( data );
  }

  void BSoundTouchDevice::startPostRequest( const QString &data )
  {
    QByteArray buffer;
    QNetworkRequest request( url );

    lg->debug( QString( "BSoundTouchDevice::startPostRequest: start POST request: %1" ).arg( url.toString() ) );
    lg->debug( QString( "BSoundTouchDevice::startPostRequest: \"%1\"" ).arg( data ) );
    //
    // post übernimmt QbyteArray, daher eine kleine Konvertierung
    //
    buffer.append( data );
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/xml" );
    // request.setHeader( QNetworkRequest::ContentTypeHeader, "text/xml" );

    QNetworkReply *reply = qnam.post( request, buffer );
    //
    // für Asynchrone Verarbeitung anbinden...
    //
    connect( reply, &QNetworkReply::finished, this, [=] { httpFinished( reply ); } );
    connect( reply, &QIODevice::readyRead, this, [=] { httpReadyRead( reply ); } );
  }

  //###########################################################################
  //#### Asyncrone HTTP Funktionen                                         ####
  //###########################################################################

  void BSoundTouchDevice::httpFinished( QNetworkReply *reply )
  {
    if ( reply->error() )
    {
      reply->deleteLater();
      reply = nullptr;
      lg->debug( "BSoundTouchDevice::httpFinished...(error)" );
      return;
    }

    const QVariant redirectionTarget = reply->attribute( QNetworkRequest::RedirectionTargetAttribute );
    if ( !redirectionTarget.isNull() )
    {
      reply->deleteLater();
      lg->debug( "BSoundTouchDevice::httpFinished...(redirected)" );
      reply = nullptr;
      return;
    }

    reply->deleteLater();
    reply = nullptr;
    lg->debug( "BSoundTouchDevice::httpFinished..." );
  }

  void BSoundTouchDevice::httpReadyRead( QNetworkReply *reply )
  {
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    lg->debug( "BSoundTouchDevice::httpReadyRead: data recived" );
    QString relpyString( reply->readAll() );
    lg->debug( relpyString );
  }

  void BSoundTouchDevice::slotAuthenticationRequired( QNetworkReply *repl, QAuthenticator *authenticator )
  {
    lg->info( "BSoundTouchDevice::slotAuthenticationRequired: raise signal for auth (if programmed....)" );
    emit sigAuthenticationRequired( repl, authenticator );
    //
    // innerhalb der SIGNAL Behandlung machen
    //
    // a uthenticator->setUser(userName;
    // authenticator->setPassword(paswort);
  }

}  // namespace radio
