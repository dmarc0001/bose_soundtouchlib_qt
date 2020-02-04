#ifndef PRESETSOBJECT_HPP
#define PRESETSOBJECT_HPP

#include <qglobal.h>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QObject>
#include <QVector>
#include <QtDebug>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT HttpPresetsObject : public IResponseObject
  {
    private:
    QVector< bose_soundtoch_lib::DevicePreset > presets;  //! Liste von Presets (aktell max 6)

    public:
    explicit HttpPresetsObject( QDomElement *domElem, QObject *parent );
    ~HttpPresetsObject() override;

    private:
    void parseContentItems( QDomNode *parentNode, bose_soundtoch_lib::DevicePreset &preset );

    public:
    //
    // GETTER
    //
    QVector< bose_soundtoch_lib::DevicePreset > getPresets() const;
  };
}  // namespace bose_soundtoch_lib
#endif  // PRESETSOBJECT_HPP
