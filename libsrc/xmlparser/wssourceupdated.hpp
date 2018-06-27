#ifndef WSSOURCEUPDATED_HPP
#define WSSOURCEUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsSourceUpdated : public IResponseObject
  {
    private:
    bool updated;

    public:
    explicit WsSourceUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsSourceUpdated() override;
  };
}  // namespace radio

#endif  // WSSOURCEUPDATED_HPP
