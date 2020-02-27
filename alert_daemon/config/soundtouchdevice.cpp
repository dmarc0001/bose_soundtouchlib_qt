#include "soundtouchdevice.hpp"

namespace bose_commserver
{
  SoundTouchDevice::SoundTouchDevice()
  {
  }

  SoundTouchDevice::SoundTouchDevice( const QString &_name, const QHostAddress _ip, qint16 _port, qint16 _wsPort, const QString &_id )
      : name( _name ), ip( _ip ), port( _port ), wsPort( _wsPort ), id( _id )
  {
  }

  QString SoundTouchDevice::getName() const
  {
    return name;
  }

  void SoundTouchDevice::setName( const QString &value )
  {
    name = value;
  }

  QHostAddress SoundTouchDevice::getIp() const
  {
    return ip;
  }

  void SoundTouchDevice::setIp( const QHostAddress &value )
  {
    ip = value;
  }

  QString SoundTouchDevice::getHostName() const
  {
    return hostName;
  }

  void SoundTouchDevice::setHostName( const QString &value )
  {
    hostName = value;
  }

  qint16 SoundTouchDevice::getPort() const
  {
    return port;
  }

  void SoundTouchDevice::setPort( const qint16 &value )
  {
    port = value;
  }

  qint16 SoundTouchDevice::getWsPort() const
  {
    return wsPort;
  }

  void SoundTouchDevice::setWsPort( const qint16 &value )
  {
    wsPort = value;
  }

  QString SoundTouchDevice::getModel() const
  {
    return model;
  }

  void SoundTouchDevice::setModel( const QString &value )
  {
    model = value;
  }

  QString SoundTouchDevice::getId() const
  {
    return id;
  }

  void SoundTouchDevice::setId( const QString &value )
  {
    id = value;
  }

}  // namespace bose_commserver
