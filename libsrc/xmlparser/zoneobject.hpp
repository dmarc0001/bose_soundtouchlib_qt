#ifndef ZONEOBJECT_HPP
#define ZONEOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <memory>
#include "../logging/Logger.hpp"
#include "soundtouch_response.hpp"
#include "iresponseobject.hpp"

namespace radio
{
  class ZoneObject : public IResponseObject
  {
    private:
    QString master;
    SoundTouchMemberList members;

    public:
    ZoneObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~ZoneObject() override;
    // Getter Funktionen
    QString getMaster() const;
    SoundTouchMemberList getMembers() const;
  };
}  // namespace radio
#endif  // ZONEOBJECT_HPP
