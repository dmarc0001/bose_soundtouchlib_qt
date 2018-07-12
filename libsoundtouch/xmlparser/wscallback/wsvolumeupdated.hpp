#ifndef WSVOLUMEUPDATE_HPP
#define WSVOLUMEUPDATE_HPP

#include <qglobal.h>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsVolumeUpdated : public IResponseObject
  {
    private:
    int targetvolume;          //! Ziel lautstärke
    int actualvolume;          //! aktuelle Lautstärke
    bool muteenabled = false;  //! mute möglich/erlaubt

    public:
    explicit WsVolumeUpdated( QDomElement *domElem, QObject *parent );
    ~WsVolumeUpdated() override;
    //
    // GETTER
    //
    int getTargetVolume() const;
    int getActualVolume() const;
    bool getMuteEnabled() const;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSVOLUMEUPDATE_HPP
