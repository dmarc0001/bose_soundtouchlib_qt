#ifndef WSNOWSELECTIONUPDATED_HPP
#define WSNOWSELECTIONUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QVector>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsNowSelectionUpdated : public IResponseObject
  {
    private:
    DevicePreset preset;

    public:
    explicit WsNowSelectionUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsNowSelectionUpdated() override;
    DevicePreset getDevicePresets() const;
  };
}  // namespace radio

#endif  // WSNOWSELECTIONUPDATED_HPP
