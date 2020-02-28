#include "soundtouchdiscover.hpp"

namespace bose_commserver
{
  /**
   * @brief SoundTouchDiscover::SoundTouchDiscover
   * @param dconfig
   * @param parent
   */
  SoundTouchDiscover::SoundTouchDiscover( std::shared_ptr< AlertAppConfig > dconfig, QObject *parent )
      : QObject( parent ), lg( dconfig->getLogger() ), config( dconfig ), zConfPtr( std::unique_ptr< QZeroConf >( new QZeroConf ) )

  {
    *lg << LDEBUG << "SoundTouchDiscover::SoundTouchDiscover: create discover object..." << endl;
    *lg << LDEBUG << "SoundTouchDiscover::SoundTouchDiscover: connect signal for errors..." << endl;
    connect( zConfPtr.get(), &QZeroConf::error, this, &SoundTouchDiscover::onError );
    *lg << LDEBUG << "SoundTouchDiscover::SoundTouchDiscover: connect signals for services..." << endl;
    connect( zConfPtr.get(), &QZeroConf::serviceAdded, this, &SoundTouchDiscover::onServiceAdded );
    connect( zConfPtr.get(), &QZeroConf::serviceRemoved, this, &SoundTouchDiscover::onServiceRemoved );
    connect( zConfPtr.get(), &QZeroConf::serviceUpdated, this, &SoundTouchDiscover::onServiceUpdated );
    *lg << LDEBUG << "SoundTouchDiscover::SoundTouchDiscover: connect signals for services...OK" << endl;
    *lg << LDEBUG << "SoundTouchDiscover::SoundTouchDiscover: start browser..." << endl;
    zConfPtr->startBrowser( SOUNDTOUCH_TCP, QAbstractSocket::IPv4Protocol );
    // zConfPtr->startBrowser( "_soundtouch._tcp.local" );
    *lg << LDEBUG << "SoundTouchDiscover::SoundTouchDiscover: start browser...OK" << endl;
    *lg << LDEBUG << "SoundTouchDiscover::SoundTouchDiscover: create discover object...OK" << endl;
  }

  /**
   * @brief SoundTouchDiscover::~SoundTouchDiscover
   */
  SoundTouchDiscover::~SoundTouchDiscover()
  {
    *lg << LDEBUG << "SoundTouchDiscover::SoundTouchDiscover: destroy object...";
    cancel();
  }

  /**
   * @brief SoundTouchDiscover::cancel
   */
  void SoundTouchDiscover::cancel()
  {
    *lg << LINFO << "SoundTouchDiscover::cancel: cancel discover devices...";
    if ( zConfPtr->browserExists() )
      zConfPtr->stopBrowser();
  }

  /**
   * @brief SoundTouchDiscover::onServiceAdded
   * @param servicePtr
   */
  void SoundTouchDiscover::onServiceAdded( QZeroConfService servicePtr )
  {
    *lg << LDEBUG << "SoundTouchDiscover::onServiceAdded: ..." << servicePtr->name() << "on host: " << servicePtr->host()
        << "domain: " << servicePtr->domain() << " type " << servicePtr->type() << "port: " << servicePtr->port()
        << servicePtr->ip().toString() << endl
        << "text: " << endl;
    for ( auto key : servicePtr->txt() )
    {
      *lg << "key: " << QString( key ) << "val: " << QString( servicePtr->txt().take( key ) ) << endl;
    }
  }

  /**
   * @brief SoundTouchDiscover::onServiceRemoved
   * @param servicePtr
   */
  void SoundTouchDiscover::onServiceRemoved( QZeroConfService servicePtr )
  {
    *lg << LDEBUG << "SoundTouchDiscover::onServiceRemoved: ..." << servicePtr->name() << "on host: " << servicePtr->host() << endl;
  }

  /**
   * @brief SoundTouchDiscover::onServiceUpdated
   * @param servicePtr
   */
  void SoundTouchDiscover::onServiceUpdated( QZeroConfService servicePtr )
  {
    *lg << LDEBUG << "SoundTouchDiscover::onServiceUpdated: ..." << servicePtr->name() << "on host: " << servicePtr->host() << endl;
  }

  /**
   * @brief SoundTouchDiscover::onError
   * @param err
   */
  void SoundTouchDiscover::onError( QZeroConf::error_t err )
  {
    switch ( err )
    {
      case QZeroConf::error_t::noError:
        *lg << LDEBUG << "NO error, all right";
        break;
      case QZeroConf::error_t::serviceRegistrationFailed:
        *lg << LCRIT << "service registration error!";
        break;
      case QZeroConf::error_t::serviceNameCollision:
        *lg << LCRIT << "service name collision!";
        break;
      case QZeroConf::error_t::browserFailed:
        *lg << LCRIT << "browser failed!";
        break;
    }
  }
}  // namespace bose_commserver
