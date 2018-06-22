#include "bsoundtouchdevice.hpp"
#include <QThread>
#include "xmlparser/xmlresultparser.hpp"

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

  void BSoundTouchDevice::setHostname( const QString &stHost )
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

  void BSoundTouchDevice::getGroup( void )
  {
    //
    // Gruppenzugehörigkeit/Infos bei SoundToch 10
    //
    setUrl( "getGroup" );
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

  void BSoundTouchDevice::selectSource( const QString &source, const QString &account = "" )
  {
    //
    // stellt am geräte eine Quelle ein
    // AUX, INTERNET, BLUETHOOTH, AMAZON, TUNEIN etc...
    // TODO: wie geht es dann weiter?
    //
    setUrl( "select" );
    lg->info( QString( "BSoundTouchDevice::selectSource: select source: %1, account: %2" ).arg( source ).arg( account ) );
    QString data( QString( "<ContentItem source=\"%1\" sourceAccount=\"%2\"></ContentItem>" ).arg( source ).arg( account ) );
    startPostRequest( data );
  }

  void BSoundTouchDevice::setBass( int bass )
  {
    //
    // Bass einstellen
    // TODO: Grenzen prüfen
    //
    setUrl( "bass" );
    lg->info( QString( "BSoundTouchDevice::setBass: set bass to %1" ).arg( bass ) );
    QString data( QString( "<bass>%1</bass>" ).arg( bass ) );
    startPostRequest( data );
  }

  void BSoundTouchDevice::setVolume( int volume )
  {
    //
    // Lautstärke einstellen (0..100)
    //
    if ( volume < 0 )
      volume = 0;
    if ( volume > 100 )
      volume = 100;
    setUrl( "volume" );
    lg->info( QString( "BSoundTouchDevice::setVolume: set volume to %1" ).arg( volume ) );
    QString data( QString( "<volume>%1</volume>" ).arg( volume ) );
    startPostRequest( data );
  }

  void BSoundTouchDevice::setZone( const QString &masterId, const SoundTouchMemberList &memberList )
  {
    //
    // erstelle eine Zohne (gemeinsames Abspielen einer Quelle) mit Mitlgliedern
    //
    lg->debug( "BSoundTouchDevice::setZone..." );
    setUrl( "setZone" );
    editZone( masterId, memberList );
  }

  void BSoundTouchDevice::addZoneSlave( const QString &masterId, const SoundTouchMemberList &memberList )
  {
    //
    // führe zu einer Zohne (gemeinsames Abspielen einer Quelle) Mitglieder hinzu
    //
    lg->debug( "BSoundTouchDevice::addZoneSlave..." );
    setUrl( "addZoneSlave" );
    editZone( masterId, memberList );
  }

  void BSoundTouchDevice::removeZoneSlave( const QString &masterId, const SoundTouchMemberList &memberList )
  {
    //
    // enferne aus einer Zohne (gemeinsames Abspielen einer Quelle) Mitglieder
    //
    lg->debug( "BSoundTouchDevice::removeZoneSlave..." );
    setUrl( "removeZoneSlave" );
    editZone( masterId, memberList );
  }

  void BSoundTouchDevice::setDeviceName( QString &name )
  {
    lg->debug( QString( "BSoundTouchDevice::removeZoneSlave: set name: \"%1\"..." ).arg( name ) );
    setUrl( "name" );
    QString data( QString( "<name>%1</name>" ).arg( name ) );
    startPostRequest( data );
  }

  //###########################################################################
  // Hilfsfuntionen                                                        ####
  //###########################################################################

  void BSoundTouchDevice::editZone( const QString &masterId, const SoundTouchMemberList &memberList )
  {
    //
    // füte zu einer Zohne (gemeinsames Abspielen einer Quelle) Mitglieder hinzu
    //
    lg->debug( "BSoundTouchDevice::editZone..." );
    //
    if ( memberList.count() > 0 )
    {
      lg->debug( QString( "BSoundTouchDevice::editZone: edit zone \"%1\" %2 members..." ).arg( masterId ).arg( memberList.count() ) );
      QString data( QString( "<zone master=\"%1\">" ).arg( masterId ) );
      QVector< SoundTouchMemberObject >::ConstIterator iter = memberList.constBegin();
      for ( ; iter != memberList.constEnd(); iter++ )
      {
        data.append( QString( "<member ipaddress=\"%1\">%2</member>" ).arg( iter->first ).arg( iter->second ) );
      }
      data.append( "</zone>" );
      startPostRequest( data );
    }
    else
    {
      lg->warn( "BSoundTouchDevice::editZone: no members for zone was given. Abort command." );
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
    connect( reply, &QNetworkReply::finished, this, [=] { slotOnHttpFinished( reply ); } );
    connect( reply, &QIODevice::readyRead, this, [=] { slotOnHttpReadyToRead( reply ); } );
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
    connect( reply, &QNetworkReply::finished, this, [=] { slotOnHttpFinished( reply ); } );
    connect( reply, &QIODevice::readyRead, this, [=] { slotOnHttpReadyToRead( reply ); } );
  }

  //###########################################################################
  //#### Asyncrone HTTP Funktionen                                         ####
  //###########################################################################

  void BSoundTouchDevice::slotOnHttpFinished( QNetworkReply *reply )
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
    //
    // hier lese ich alles am Ende der Übertagung (die Datenmenge ist klein)
    //
    QString relpyString( reply->readAll() );
    lg->debug( relpyString );
    XmlResultParser xmlParser( lg, relpyString, this );
    if ( xmlParser.hasError() )
    {
      lg->crit( QString( "BSoundTouchDevice::slotOnHttpFinished: error while %1" ).arg( reply->request().url().toString() ) );
      lg->crit( QString( "BSoundTouchDevice::slotOnHttpFinished: %1" ).arg( xmlParser.getErrorString() ) );
    }
    else
    {
      std::shared_ptr< IResponseObject > response = xmlParser.getResultObject();
      if ( response.get() == nullptr )
      {
        lg->crit( "BSoundTouchDevice::slotOnHttpFinished: no response from parser" );
      }
      else
      {
        lg->debug( QString( "BSoundTouchDevice::slotOnHttpFinished: result object type %1" )
                       .arg( static_cast< qint8 >( response->getResultType() ) ) );
        // TODO: hier verarbeiten
      }
    }
    reply->deleteLater();
    reply = nullptr;
    lg->debug( "BSoundTouchDevice::httpFinished..." );
  }

  void BSoundTouchDevice::slotOnHttpReadyToRead( QNetworkReply * )
  {
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    lg->debug( "BSoundTouchDevice::httpReadyRead: data recived" );
    // QString relpyString( reply->readAll() );
    // lg->debug( relpyString );
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
