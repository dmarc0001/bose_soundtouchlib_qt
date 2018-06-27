#ifndef WSZONEUPDATED_HPP
#define WSZONEUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsZoneUpdated : public IResponseObject
  {
    private:
    bool updatet;

    public:
    explicit WsZoneUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsZoneUpdated() override;
  };
}  // namespace radio

#endif  // WSZONEUPDATED_HPP
