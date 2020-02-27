#ifndef SOUNDBARBROWSER_HPP
#define SOUNDBARBROWSER_HPP

#include <QtZeroConf/qzeroconf.h>

#include <QDebug>
#include <QObject>
#include <memory>

constexpr char SOUNDTOUCH_TCP[] = "_soundtouch._tcp.local";

class SoundTouchDiscover : public QObject
{
  Q_OBJECT
  private:
  std::unique_ptr< QZeroConf > zConfPtr;

  public:
  explicit SoundTouchDiscover( QObject *parent = nullptr );
  ~SoundTouchDiscover();
  void run();

  signals:
  private slots:
  void onError( QZeroConf::error_t );
  void onServiceAdded( QZeroConfService );
  void onServiceRemoved( QZeroConfService );
  void onServiceUpdated( QZeroConfService );
};

#endif  // SOUNDBARBROWSER_HPP
