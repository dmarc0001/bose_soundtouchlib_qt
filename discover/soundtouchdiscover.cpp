#include "soundtouchdiscover.hpp"

SoundTouchDiscover::SoundTouchDiscover( QObject *parent )
    : QObject( parent ), zConfPtr( std::unique_ptr< QZeroConf >( new QZeroConf ) )

{
  qDebug() << "SoundTouchDiscover::SoundTouchDiscover: create object...";
}

SoundTouchDiscover::~SoundTouchDiscover()
{
  qDebug() << "SoundTouchDiscover::SoundTouchDiscover: destroy object...";
}

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
  // zConfPtr->startBrowser( "_test._tcp", AVAHI_PROTO_INET );
  // zConfPtr->startBrowser( "_test._tcp" );
  // zConfPtr->startBrowser( "_soundtouch._tcp" );
  zConfPtr->startBrowser( "_soundtouch._tcp.local" );
  qDebug() << "SoundTouchDiscover::run: start browser...OK";
  // zConfPtr->startBrowser( SOUNDTOUCH_TCP );
}

void SoundTouchDiscover::onServiceAdded( QZeroConfService servicePtr )
{
  qDebug() << "SoundTouchDiscover::onServiceAdded: ..." << servicePtr->name() << "on host: " << servicePtr->host();
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
  qDebug() << "ALERT  error: (" << err << ")";
}
