#ifndef SOUNDTOUCHDEVICE_HPP
#define SOUNDTOUCHDEVICE_HPP
#include <QHostAddress>
#include <QObject>
#include <QString>

namespace bose_commserver
{
  class SoundTouchDevice;
  using SoundTouchDevicesList = QList< SoundTouchDevice >;

  class SoundTouchDevice
  {
    private:
    QString name{"localhost"};  //! Name des Gerätes
    QHostAddress ip;            //! IP Adresse
    QString hostName{""};       //! Hostname nach avahi
    quint16 port{8090};         //! port für Kommandos
    quint16 wsPort{8080};       //! port für websocket
    QString model{"unknown"};   //! informativ das Modell
    QString id{"00000000"};     //! Id / MAC

    public:
    SoundTouchDevice();
    SoundTouchDevice( const QString &_name, const QHostAddress _ip, quint16 _port, quint16 _wsPort, const QString &id );
    QString &getName();
    void setName( const QString &value );
    QHostAddress &getIp();
    void setIp( const QHostAddress &value );
    QString &getHostName();
    void setHostName( const QString &value );
    quint16 getPort() const;
    void setPort( const quint16 &value );
    quint16 getWsPort() const;
    void setWsPort( const quint16 &value );
    QString &getModel();
    void setModel( const QString &value );
    QString &getId();
    void setId( const QString &value );
  };
}  // namespace bose_commserver
#endif  // SOUNDTOUCHDEVICE_HPP
