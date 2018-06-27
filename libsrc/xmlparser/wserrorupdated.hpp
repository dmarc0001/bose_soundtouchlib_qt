#ifndef WSERRORUPDATED_HPP
#define WSERRORUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsErrorUpdated : public IResponseObject
  {
    private:
    ResponseError error;

    public:
    explicit WsErrorUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsErrorUpdated() override;
  };
}  // namespace radio

#endif  // WSERRORUPDATED_HPP
