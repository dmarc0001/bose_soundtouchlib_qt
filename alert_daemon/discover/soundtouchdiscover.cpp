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
    if ( zConfPtr->browserExists() )
    {
      *lg << LINFO << "SoundTouch discovering for devices started..." << endl;
    }
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
    *lg << LINFO << "SoundTouch cancel discover devices...";
    if ( zConfPtr->browserExists() )
      zConfPtr->stopBrowser();
  }

  /**
   * @brief SoundTouchDiscover::onServiceAdded
   * @param servicePtr
   */
  void SoundTouchDiscover::onServiceAdded( QZeroConfService servicePtr )
  {
    QMutexLocker locker( &logMutex );
    bool isNewDevice = true;
    //
    // etwas zum lesen
    //
    *lg << LDEBUG << "SoundTouchDiscover::onServiceAdded: " << servicePtr->name() << " on host: " << servicePtr->host()
        << " domain: " << servicePtr->domain() << " port: " << servicePtr->port() << " IP: " << servicePtr->ip().toString() << endl;
    /*
    *lg << LDEBUG << "SoundTouchDiscover::onServiceAdded: " << servicePtr->name() << " on host: " << servicePtr->host()
        << " domain: " << servicePtr->domain() << " type " << servicePtr->type() << " port: " << servicePtr->port()
        << " IP: " << servicePtr->ip().toString() << endl
        << " text: " << endl;
    //
    // die empfangenen Werte zeigen
    //
    for ( auto key : servicePtr->txt().keys() )
    {
      *lg << " key: <" << QString( key ) << "> val: <" << QString( ( servicePtr->txt() ).take( key ) ) << ">" << endl;
    }
    */
    //
    // in der config schauen, ob das Teil schon gespeichert war
    //
    SoundTouchDevicesList &dList = config->getAvailDevices();
    for ( auto &currDev : dList )
    {
      if ( currDev.getName().compare( servicePtr->name() ) == 0 )
      {
        *lg << LDEBUG << "SoundTouchDiscover::onServiceAdded:: device is in the list, ignore..." << endl;
        //
        // schon gefunden, ignorieren
        //
        isNewDevice = false;
        break;
      }
    }
    //
    // war das Gerät neu in der Liste
    //
    if ( isNewDevice )
    {
      SoundTouchDevice newDev( servicePtr->name(), servicePtr->ip(), servicePtr->port(),
                               static_cast< quint16 >( config->getDefaultWsPort().toInt() ), servicePtr->txt().take( "MAC" ) );
      newDev.setHostName( servicePtr->host() );
      newDev.setModel( servicePtr->txt().take( "MODEL" ) );
      *lg << LINFO << "new service for device <" << servicePtr->name() << "> addet to config." << endl;
      dList.append( newDev );
    }
  }

  /**
   * @brief SoundTouchDiscover::onServiceRemoved
   * @param servicePtr
   */
  void SoundTouchDiscover::onServiceRemoved( QZeroConfService servicePtr )
  {
    QMutexLocker locker( &logMutex );
    *lg << LDEBUG << "SoundTouchDiscover::onServiceRemoved: " << servicePtr->name() << " on host: " << servicePtr->host() << endl;
    //
    // in der config schauen, ob das Teil schon gespeichert war
    //
    SoundTouchDevicesList &dList = config->getAvailDevices();
    for ( auto it = dList.begin(); it != dList.end(); it++ )
    {
      //
      // iteriere über die Liste
      //
      if ( it->getName().compare( servicePtr->name() ) == 0 )
      {
        //
        // wenn das Gerät gefunden wurde
        // entferne es
        //
        it = dList.erase( it );
        *lg << LINFO << "device <" << servicePtr->name() << "> on host: " << servicePtr->host() << " was removed from config." << endl;
      }
    }
  }

  /**
   * @brief SoundTouchDiscover::onServiceUpdated
   * @param servicePtr
   */
  void SoundTouchDiscover::onServiceUpdated( QZeroConfService servicePtr )
  {
    *lg << LDEBUG << "SoundTouchDiscover::onServiceUpdated: ..." << servicePtr->name() << " on host: " << servicePtr->host() << endl;
    //
    // gucke mal, ob das in der Liste ist...
    //
    SoundTouchDevicesList &dList = config->getAvailDevices();
    for ( auto it = dList.begin(); it != dList.end(); it++ )
    {
      //
      // iteriere über die Liste
      //
      if ( it->getName().compare( servicePtr->name() ) == 0 )
      {
        //
        // wenn das Gerät geunden wurde
        // UPDATE
        //
        it->setIp( servicePtr->ip() );
        it->setPort( servicePtr->port() );
        it->setId( servicePtr->txt().take( "MAC" ) );
        *lg << LINFO << "device <" << servicePtr->name() << "> on host: " << servicePtr->host() << " was updated in the config."
            << endl;
        break;
      }
    }
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
