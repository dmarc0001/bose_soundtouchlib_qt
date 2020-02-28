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
    qint16 port{8090};          //! port für Kommandos
    qint16 wsPort{8080};        //! port für websocket
    QString model{"unknown"};   //! informativ das Modell
    QString id{"00000000"};     //! Id / MAC

    public:
    SoundTouchDevice();
    SoundTouchDevice( const QString &_name, const QHostAddress _ip, qint16 _port, qint16 _wsPort, const QString &id );
    QString getName() const;
    void setName( const QString &value );
    QHostAddress getIp() const;
    void setIp( const QHostAddress &value );
    QString getHostName() const;
    void setHostName( const QString &value );
    qint16 getPort() const;
    void setPort( const qint16 &value );
    qint16 getWsPort() const;
    void setWsPort( const qint16 &value );
    QString getModel() const;
    void setModel( const QString &value );
    QString getId() const;
    void setId( const QString &value );
  };
}  // namespace bose_commserver
#endif  // SOUNDTOUCHDEVICE_HPP
