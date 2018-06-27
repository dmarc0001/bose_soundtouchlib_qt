#ifndef WSINFOUPDATED_HPP
#define WSINFOUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsInfoUpdated : public IResponseObject
  {
    private:
    bool updatet;

    public:
    explicit WsInfoUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsInfoUpdated() override;
  };
}  // namespace radio

#endif  // WSINFOUPDATED_HPP
