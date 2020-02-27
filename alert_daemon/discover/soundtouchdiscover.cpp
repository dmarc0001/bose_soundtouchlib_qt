#include "soundtouchdiscover.hpp"

namespace bose_commserver
{
  SoundTouchDiscover::SoundTouchDiscover( std::shared_ptr< AlertAppConfig > dconfig, QObject *parent )
      : QObject( parent ), lg( dconfig->getLogger() ), config( dconfig ), zConfPtr( std::unique_ptr< QZeroConf >( new QZeroConf ) )

  {
    lg->debug( "SoundTouchDiscover::SoundTouchDiscover: create discover object..." );
    lg->debug( "SoundTouchDiscover::SoundTouchDiscover: connect signal for errors..." );
    connect( zConfPtr.get(), &QZeroConf::error, this, &SoundTouchDiscover::onError );
    lg->debug( "SoundTouchDiscover::SoundTouchDiscover: connect signals for services..." );
    connect( zConfPtr.get(), &QZeroConf::serviceAdded, this, &SoundTouchDiscover::onServiceAdded );
    connect( zConfPtr.get(), &QZeroConf::serviceRemoved, this, &SoundTouchDiscover::onServiceRemoved );
    connect( zConfPtr.get(), &QZeroConf::serviceUpdated, this, &SoundTouchDiscover::onServiceUpdated );
    lg->debug( "SoundTouchDiscover::SoundTouchDiscover: connect signals for services...OK" );
    lg->debug( "SoundTouchDiscover::SoundTouchDiscover: start browser..." );
    zConfPtr->startBrowser( SOUNDTOUCH_TCP, QAbstractSocket::IPv4Protocol );
    // zConfPtr->startBrowser( "_soundtouch._tcp.local" );
    lg->debug( "SoundTouchDiscover::SoundTouchDiscover: start browser...OK" );
    lg->debug( "SoundTouchDiscover::SoundTouchDiscover: create discover object...OK" );
  }

  SoundTouchDiscover::~SoundTouchDiscover()
  {
    lg->debug( "SoundTouchDiscover::SoundTouchDiscover: destroy object..." );
    cancel();
  }

  void SoundTouchDiscover::cancel()
  {
    lg->info( "SoundTouchDiscover::cancel: cancel discover devices..." );
    if ( zConfPtr->browserExists() )
      zConfPtr->stopBrowser();
  }
  /*
  void SoundTouchDiscover::run()
  {
    qDebug() << "SoundTouchDiscover::run: connect signal for errors...";
    connect( zConfPtr.get(), &QZeroConf::error, this, &SoundTouchDiscover::onError );
    qDebug() << "SoundTouchDiscover::run: connect signals for services...";
    connect( zConfPtr.get(), &QZeroConf::serviceAdded, this, &SoundTouchDiscover::onServiceAdded );
    connect( zConfPtr.get(), &QZeroConf::serviceRemoved, this, &SoundTouchDiscover::onServiceRemoved );
    connect( zConfPtr.get(), &QZeroConf::serviceUpdated, this, &SoundTouchDiscover::onServiceUpdated );
    qDebug() << "SoundTouchDiscover::run: connect signals for services...OK";
    qDebug() << "SoundTouchDiscover::run: start browser...";
    zConfPtr->startBrowser( SOUNDTOUCH_TCP, QAbstractSocket::IPv4Protocol );
    // zConfPtr->startBrowser( "_soundtouch._tcp.local" );
    qDebug() << "SoundTouchDiscover::run: start browser...OK";
  }
  */
  void SoundTouchDiscover::onServiceAdded( QZeroConfService servicePtr )
  {
    qDebug() << "SoundTouchDiscover::onServiceAdded: ..." << servicePtr->name() << "on host: " << servicePtr->host()
             << "domain: " << servicePtr->domain() << " type " << servicePtr->type() << "port: " << servicePtr->port()
             << servicePtr->ip() << "text: " << servicePtr->txt();
  }

  void SoundTouchDiscover::onServiceRemoved( QZeroConfService servicePtr )
  {
    qDebug() << "SoundTouchDiscover::onServiceRemoved: ..." << servicePtr->name() << "on host: " << servicePtr->host();
  }

  void SoundTouchDiscover::onServiceUpdated( QZeroConfService servicePtr )
  {
    qDebug() << "SoundTouchDiscover::onServiceUpdated: ..." << servicePtr->name() << "on host: " << servicePtr->host();
  }

  void SoundTouchDiscover::onError( QZeroConf::error_t err )
  {
    switch ( err )
    {
      case QZeroConf::error_t::noError:
        qDebug() << "NO error, all right";
        break;
      case QZeroConf::error_t::serviceRegistrationFailed:
        qDebug() << "service registration error!";
        break;
      case QZeroConf::error_t::serviceNameCollision:
        qDebug() << "service name collision!";
        break;
      case QZeroConf::error_t::browserFailed:
        qDebug() << "browser failed!";
        break;
    }
  }
}  // namespace bose_commserver
