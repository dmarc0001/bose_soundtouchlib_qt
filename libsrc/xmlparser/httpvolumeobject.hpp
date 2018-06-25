#ifndef VOLUMEOBJECT_HPP
#define VOLUMEOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include "../logging/Logger.hpp"
#include "iresponseobject.hpp"

namespace radio
{
  class HttpVolumeObject : public IResponseObject
  {
    private:
    QString deviceId;
    int volume;
    int targetvolume;
    int actualvolume;
    bool muteenabled = false;

    public:
    explicit HttpVolumeObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpVolumeObject() override;
    QString getDeviceId() const;
    int getVolume() const;
    int getTargetVolume() const;
    int getActualVolume() const;
    bool getMuteEnabled() const;
  };
}  // namespace radio
#endif  // VOLUMEOBJECT_HPP
