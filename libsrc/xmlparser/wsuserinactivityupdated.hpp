#ifndef WSUSERINACTIVITYUPDATED_HPP
#define WSUSERINACTIVITYUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsUserInactivityUpdated : public IResponseObject
  {
    public:
    explicit WsUserInactivityUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsUserInactivityUpdated() override;
  };
}  // namespace radio

#endif  // WSUSERINACTIVITYUPDATED_HPP
