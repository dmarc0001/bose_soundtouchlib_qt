#ifndef WSPRESETUPDATEOBJECT_HPP
#define WSPRESETUPDATEOBJECT_HPP

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
  class WsPresetUpdateObject : public IResponseObject
  {
    private:
    QVector< DevicePreset > devicePresets;  //! Liste mit Presets des Gerätes/der Geräte. Aktuell 6 stück möglich

    public:
    explicit WsPresetUpdateObject( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsPresetUpdateObject() override;
    //
    // GETTER
    //
    QVector< DevicePreset > getDevicePresets( void ) const;

    private:
    void parseAndAddPreset( void );  //! einien preset der Liste zufügen
  };
}  // namespace bose_soundtoch_lib
#endif  // WSPRESETUPDATEOBJECT_HPP
