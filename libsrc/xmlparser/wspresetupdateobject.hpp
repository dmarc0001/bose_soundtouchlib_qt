#ifndef WSPRESETUPDATEOBJECT_HPP
#define WSPRESETUPDATEOBJECT_HPP

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
  class WsPresetUpdateObject : public IResponseObject
  {
    private:
    QVector< DevicePreset > devicePresets;

    public:
    explicit WsPresetUpdateObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsPresetUpdateObject() override;
    QVector< DevicePreset > getDevicePresets( void ) const;

    private:
    void parseAndAddPreset( void );
  };
}  // namespace radio
#endif  // WSPRESETUPDATEOBJECT_HPP
