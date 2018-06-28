#ifndef WSNOWSELECTIONUPDATED_HPP
#define WSNOWSELECTIONUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QVector>
#include <QtDebug>
#include <memory>
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsNowSelectionUpdated : public IResponseObject
  {
    private:
    DevicePreset preset;  //! ein Preset wurde verändert

    public:
    explicit WsNowSelectionUpdated( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsNowSelectionUpdated() override;
    //
    // GETTER
    //
    DevicePreset getDevicePresets() const;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSNOWSELECTIONUPDATED_HPP
