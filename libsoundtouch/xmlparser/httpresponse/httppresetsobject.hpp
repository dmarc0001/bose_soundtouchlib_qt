#ifndef PRESETSOBJECT_HPP
#define PRESETSOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include <QtDebug>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class HttpPresetsObject : public IResponseObject
  {
    private:
    QVector< bose_soundtoch_lib::DevicePreset > presets;  //! Liste von Presets (aktell max 6)

    public:
    explicit HttpPresetsObject( QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpPresetsObject() override;
    //
    // GETTER
    //
    QVector< bose_soundtoch_lib::DevicePreset > getPresets() const;
  };
}  // namespace bose_soundtoch_lib
#endif  // PRESETSOBJECT_HPP
