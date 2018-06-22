#ifndef PRESETSOBJECT_HPP
#define PRESETSOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace radio
{
  class PresetsObject : public IResponseObject
  {
    public:
    QVector< radio::DevicePreset > presets;

    explicit PresetsObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~PresetsObject() override;

    private:
    void parseContentItem( radio::DevicePreset &preset );
  };
}  // namespace radio
#endif  // PRESETSOBJECT_HPP
