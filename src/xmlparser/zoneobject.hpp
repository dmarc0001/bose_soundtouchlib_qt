#ifndef ZONEOBJECT_HPP
#define ZONEOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <memory>
#include "../global/bsoundtouch_global.hpp"
#include "../logging/Logger.hpp"
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
  };
}  // namespace radio
#endif  // ZONEOBJECT_HPP
