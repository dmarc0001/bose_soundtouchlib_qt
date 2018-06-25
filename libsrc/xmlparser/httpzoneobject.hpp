#ifndef ZONEOBJECT_HPP
#define ZONEOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <memory>
#include "../logging/Logger.hpp"
#include "iresponseobject.hpp"
#include "soundtouch_response.hpp"

namespace radio
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
