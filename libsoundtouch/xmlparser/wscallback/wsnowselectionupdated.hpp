#ifndef WSNOWSELECTIONUPDATED_HPP
#define WSNOWSELECTIONUPDATED_HPP

#include <qglobal.h>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QObject>
#include <QString>
#include <QVector>
#include <QtDebug>
#include <memory>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT WsNowSelectionUpdated : public IResponseObject
  {
    private:
    DevicePreset preset;  //! ein Preset wurde verändert

    public:
    explicit WsNowSelectionUpdated( QDomElement *domElem, QObject *parent );
    ~WsNowSelectionUpdated() override;
    //
    // GETTER
    //
    DevicePreset getDevicePresets() const;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSNOWSELECTIONUPDATED_HPP
