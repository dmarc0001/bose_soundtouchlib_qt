#include "bsoundtouchdevice.hpp"
#include <QThread>
#include "xmlparser/xmlresultparser.hpp"
#include "xmlparser/xmlupdateparser.hpp"

namespace bose_soundtoch_lib
{
  const QString BSoundTouchDevice::version = QString( "%1.%2.%3" ).arg( VMAJOR ).arg( VMINOR ).arg( VPATCH );

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
  BSoundTouchDevice::BSoundTouchDevice( QString &stHost, qint16 stWSPort, qint16 stHttpPort, QObject *parent )
      : QObject( parent )
      , hostname( stHost )
      , wsPort( stWSPort )
      , httpPort( stHttpPort )
      , webSocket( nullptr )
      , threshold( QtMsgType::QtFatalMsg )
  {
    qDebug() << "...";
    connect( &qnam, &QNetworkAccessManager::authenticationRequired, this, &BSoundTouchDevice::slotAuthenticationRequired );
  }

  /**
   * @brief BSoundTouchDevice::~BSoundTouchDevice
   */
  BSoundTouchDevice::~BSoundTouchDevice()
  {
    qDebug() << "...";
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

  void BSoundTouchDevice::setKey( bose_key whichkey, bose_keystate keystate, QString )
  {
    QString data;
    //
    qDebug() << BSoundTouchDevice::keynames[ static_cast< int >( whichkey ) ] << " "
             << BSoundTouchDevice::keystati[ static_cast< int >( keystate ) ] << " to " << hostname;
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
        qInfo() << "power switch (toggle)...";
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
        qInfo() << "power switch (" << keystati[ static_cast< int >( keystate ) ] << ")...";
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
    qInfo() << "select source: " << source << "account: " << account;
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
    qInfo() << "set bass to " << bass;
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
    qInfo() << "set volume to " << volume;
    QString data( QString( "<volume>%1</volume>" ).arg( volume ) );
    startPostRequest( data );
  }

  void BSoundTouchDevice::setZone( const QString &masterId, const SoundTouchMemberList &memberList )
  {
    //
    // erstelle eine Zohne (gemeinsames Abspielen einer Quelle) mit Mitlgliedern
    //
    qInfo() << "set zone...";
    setUrl( "setZone" );
    editZone( masterId, memberList );
  }

  void BSoundTouchDevice::addZoneSlave( const QString &masterId, const SoundTouchMemberList &memberList )
  {
    //
    // führe zu einer Zohne (gemeinsames Abspielen einer Quelle) Mitglieder hinzu
    //
    qInfo() << "add zone slave(s)...";
    setUrl( "addZoneSlave" );
    editZone( masterId, memberList );
  }

  void BSoundTouchDevice::removeZoneSlave( const QString &masterId, const SoundTouchMemberList &memberList )
  {
    //
    // enferne aus einer Zohne (gemeinsames Abspielen einer Quelle) Mitglieder
    //
    qInfo() << "remove zone slave(s)...";
    setUrl( "removeZoneSlave" );
    editZone( masterId, memberList );
  }

  void BSoundTouchDevice::setDeviceName( QString &name )
  {
    qInfo() << "set devce name to: " << name;
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
    qDebug() << "...";
    //
    if ( memberList.count() > 0 )
    {
      qDebug() << "edit zone \"" << masterId << "\": " << memberList.count() << " members...";
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
      qWarning() << "no members for zone was given. Abort command.";
    }
  }

  //###########################################################################
  //#### hilfsfunktionen GET/POST                                          ####
  //###########################################################################

  void BSoundTouchDevice::startGetRequest( void )
  {
    qDebug() << "GET Request: " << url.toString();
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

    qDebug() << "start POST request: " << url.toString();
    qDebug() << "start POST data: " << data;
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
    qDebug() << "...";
    if ( reply->error() )
    {
      qWarning() << "reply error: " << reply->error();
      reply->deleteLater();
      reply = nullptr;
      return;
    }

    const QVariant redirectionTarget = reply->attribute( QNetworkRequest::RedirectionTargetAttribute );
    if ( !redirectionTarget.isNull() )
    {
      reply->deleteLater();
      qDebug() << "redirected...";
      reply = nullptr;
      return;
    }
    //
    // hier lese ich alles am Ende der Übertagung (die Datenmenge ist klein)
    //
    QString relpyString( reply->readAll() );
    qDebug() << relpyString;
    XmlResultParser xmlParser( relpyString, this );
    if ( xmlParser.hasError() )
    {
      qCritical() << "error while " << reply->request().url().toString();
      qCritical() << "error: " << xmlParser.getErrorString();
    }
    else
    {
      std::shared_ptr< IResponseObject > response = xmlParser.getResultObject();
      if ( response.get() == nullptr )
      {
        qCritical() << "no response from xml response parser";
      }
      else
      {
        qDebug() << "result object type: " << response->getResultTypeName();
        //
        // TODO: hier verarbeiten
        //
      }
    }
    reply->deleteLater();
    reply = nullptr;
  }

  void BSoundTouchDevice::slotOnHttpReadyToRead( QNetworkReply * )
  {
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    qDebug() << "data recived...";
    // QString relpyString( reply->readAll() );
    // lg->debug( relpyString );
  }

  void BSoundTouchDevice::slotAuthenticationRequired( QNetworkReply *repl, QAuthenticator *authenticator )
  {
    qInfo() << "raise signal for auth (if programmed....)";
    emit sigAuthenticationRequired( repl, authenticator );
    //
    // innerhalb der SIGNAL Behandlung machen
    //
    // a uthenticator->setUser(userName;
    // authenticator->setPassword(paswort);
  }

  void BSoundTouchDevice::addVolumeListener( void )
  {
    connectWs();
  }

  void BSoundTouchDevice::connectWs( void )
  {
    if ( webSocket.get() == nullptr )
    {
      webSocket = std::unique_ptr< BWebsocket >( new BWebsocket( hostname, wsPort, this ) );
      //
      // signale verbinden
      //
      connect( webSocket.get(), &BWebsocket::sigOnWSConnected, this, &BSoundTouchDevice::slotOnWSConnected );
      connect( webSocket.get(), &BWebsocket::sigOnWSDisConnected, this, &BSoundTouchDevice::slotOnWSDisConnected );
      connect( webSocket.get(), &BWebsocket::sigOnWSTextMessageReceived, this, &BSoundTouchDevice::slotOnWSTextMessageReceived );
      // nach open sollte sowas kommen: <SoundTouchSdkInfo serverVersion="4" serverBuild="trunk r42017 v4 epdbuild cepeswbld02" />
      webSocket->open();
    }
  }

  void BSoundTouchDevice::slotOnWSConnected( void )
  {
    qDebug() << "...";
    emit sigOnWSConnected();
  }

  void BSoundTouchDevice::slotOnWSDisConnected( void )
  {
    qDebug() << "...";
    emit sigOnWSDisConnected();
  }

  void BSoundTouchDevice::slotOnWSTextMessageReceived( QString message )
  {
    qDebug() << "...";
    //
    // die nachricht nach bekannten Obketken parsen
    //

    XMLUpdateParser xmlParser( message, this );
    if ( xmlParser.hasError() )
    {
      qCritical() << "xml parser error: " << xmlParser.getErrorString();
    }
    else
    {
      std::shared_ptr< IResponseObject > response = xmlParser.getResultObject();
      if ( response.get() == nullptr )
      {
        qCritical() << "no response from xml update parser";
      }
      else
      {
        qDebug() << "result object type: " << response->getResultTypeName();
        //
        // TODO: hier verarbeiten
        //
      }
    }
    emit sigOnWSTextMessageReceived( message );
  }

  void BSoundTouchDevice::myMessageOutput( QtMsgType type, const QMessageLogContext &context, const QString &msg )
  {
    QByteArray localMsg = msg.toLocal8Bit();
    switch ( type )
    {
      case QtDebugMsg:
        fprintf( stdout, "DEBUG: %s (%s)\n", localMsg.constData(), context.function );
        fflush( stdout );
        // fprintf( stdout, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
      case QtInfoMsg:
        fprintf( stdout, "INFO: %s\n", localMsg.constData() );
        // fprintf( stdout, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
      case QtWarningMsg:
        fprintf( stdout, "WARNING: %s\n", localMsg.constData() );
        // fprintf( stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
      case QtCriticalMsg:
        fprintf( stderr, "CRITICAL: %s (%s:%u)\n", localMsg.constData(), context.file, context.line );
        fflush( stderr );
        // fprintf( stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
      case QtFatalMsg:
        fprintf( stderr, "FATAL: %s (%s:%u)\n", localMsg.constData(), context.file, context.line );
        fflush( stderr );
        // fprintf( stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
    }
  }

}  // namespace bose_soundtoch_lib
