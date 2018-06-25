#ifndef PRESETSOBJECT_HPP
#define PRESETSOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include "../logging/Logger.hpp"
#include "iresponseobject.hpp"
#include "soundtouch_response.hpp"

namespace radio
{
  class HttpPresetsObject : public IResponseObject
  {
    private:
    QVector< radio::DevicePreset > presets;

    public:
    explicit HttpPresetsObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpPresetsObject() override;
    // Getter Funktionen
    QVector< radio::DevicePreset > getPresets() const;

    private:
    void parseContentItem( radio::DevicePreset &preset );
  };
}  // namespace radio
#endif  // PRESETSOBJECT_HPP
