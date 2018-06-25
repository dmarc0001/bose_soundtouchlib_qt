#ifndef PRESETSOBJECT_HPP
#define PRESETSOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include "../logging/Logger.hpp"
#include "soundtouch_response.hpp"
#include "iresponseobject.hpp"

namespace radio
{
  class PresetsObject : public IResponseObject
  {
    private:
    QVector< radio::DevicePreset > presets;

    public:
    explicit PresetsObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~PresetsObject() override;
    // Getter Funktionen
    QVector< radio::DevicePreset > getPresets() const;

    private:
    void parseContentItem( radio::DevicePreset &preset );
  };
}  // namespace radio
#endif  // PRESETSOBJECT_HPP
