#include "soundtouchdevice.hpp"

namespace bose_commserver
{
  SoundTouchDevice::SoundTouchDevice()
  {
  }

  SoundTouchDevice::SoundTouchDevice( const QString &_name,
                                      const QHostAddress _ip,
                                      quint16 _port,
                                      quint16 _wsPort,
                                      const QString &_id )
      : name( _name ), ip( _ip ), port( _port ), wsPort( _wsPort ), id( _id )
  {
  }

  QString &SoundTouchDevice::getName()
  {
    return name;
  }

  void SoundTouchDevice::setName( const QString &value )
  {
    name = value;
  }

  QHostAddress &SoundTouchDevice::getIp()
  {
    return ip;
  }

  void SoundTouchDevice::setIp( const QHostAddress &value )
  {
    ip = value;
  }

  QString &SoundTouchDevice::getHostName()
  {
    return hostName;
  }

  void SoundTouchDevice::setHostName( const QString &value )
  {
    hostName = value;
  }

  quint16 SoundTouchDevice::getPort() const
  {
    return port;
  }

  void SoundTouchDevice::setPort( const quint16 &value )
  {
    port = value;
  }

  quint16 SoundTouchDevice::getWsPort() const
  {
    return wsPort;
  }

  void SoundTouchDevice::setWsPort( const quint16 &value )
  {
    wsPort = value;
  }

  QString &SoundTouchDevice::getModel()
  {
    return model;
  }

  void SoundTouchDevice::setModel( const QString &value )
  {
    model = value;
  }

  QString &SoundTouchDevice::getId()
  {
    return id;
  }

  void SoundTouchDevice::setId( const QString &value )
  {
    id = value;
  }

}  // namespace bose_commserver
