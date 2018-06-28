#ifndef VOLUMEOBJECT_HPP
#define VOLUMEOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QtDebug>
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class HttpVolumeObject : public IResponseObject
  {
    private:
    int targetvolume;  //! Lautstärke gewünscht
    int actualvolume;  //! Aktuelle Lautstärke
    bool muteenabled = false;

    public:
    explicit HttpVolumeObject( QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpVolumeObject() override;
    //
    // GETTER
    //
    int getTargetVolume() const;
    int getActualVolume() const;
    bool getMuteEnabled() const;
  };
}  // namespace bose_soundtoch_lib
#endif  // VOLUMEOBJECT_HPP
