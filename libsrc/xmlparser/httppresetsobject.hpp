#ifndef PRESETSOBJECT_HPP
#define PRESETSOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class HttpPresetsObject : public IResponseObject
  {
    private:
    QVector< bose_soundtoch_lib::DevicePreset > presets;

    public:
    explicit HttpPresetsObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpPresetsObject() override;
    // Getter Funktionen
    QVector< bose_soundtoch_lib::DevicePreset > getPresets() const;
  };
}  // namespace radio
#endif  // PRESETSOBJECT_HPP
