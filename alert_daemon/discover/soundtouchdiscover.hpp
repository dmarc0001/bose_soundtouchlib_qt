#ifndef SOUNDBARBROWSER_HPP
#define SOUNDBARBROWSER_HPP

#include <qtzeroconf/qzeroconf.h>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <memory>
#include "config/common_def.hpp"
#include "config/soundtouchdevice.hpp"
#include "logging/logger.hpp"

namespace bose_commserver
{
  //! das ist die Kennung für Soundtouch Geräte
  constexpr char SOUNDTOUCH_TCP[] = "_soundtouch._tcp";

  class SoundTouchDiscover : public QObject
  {
    Q_OBJECT
    private:
    LoggerPtr lg;                              //! wenn ein logger erstellt wurde
    std::shared_ptr< AlertAppConfig > config;  //! das Konfigurationsobjekt
    std::unique_ptr< QZeroConf > zConfPtr;     //! Objekt zum suchen von Geräten
    QMutex logMutex;                           //! ein paar Sachen serialisieren

    public:
    explicit SoundTouchDiscover( std::shared_ptr< AlertAppConfig > dconfig, QObject *parent = nullptr );
    ~SoundTouchDiscover();
    // void run();
    void cancel();

    signals:
    void sigDevicesgChanged();

    private slots:
    void onError( QZeroConf::error_t );
    void onServiceAdded( QZeroConfService );
    void onServiceRemoved( QZeroConfService );
    void onServiceUpdated( QZeroConfService );
  };
}  // namespace bose_commserver
#endif  // SOUNDBARBROWSER_HPP
