#ifndef WSUSERACTIVITYUPDATED_HPP
#define WSUSERACTIVITYUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsUserActivityUpdated : public IResponseObject
  {
    public:
    explicit WsUserActivityUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsUserActivityUpdated() override;
  };
}  // namespace radio

#endif  // WSUSERACTIVITYUPDATED_HPP
