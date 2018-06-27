#ifndef ZONEOBJECT_HPP
#define ZONEOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class HttpZoneObject : public IResponseObject
  {
    private:
    QString master;
    SoundTouchMemberList members;

    public:
    explicit HttpZoneObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpZoneObject() override;
    // Getter Funktionen
    QString getMaster() const;
    SoundTouchMemberList getMembers() const;
  };
}  // namespace radio
#endif  // ZONEOBJECT_HPP
