#ifndef WSNAMEUPDATED_HPP
#define WSNAMEUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsNameUpdated : public IResponseObject
  {
    private:
    bool updatet;

    public:
    explicit WsNameUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsNameUpdated() override;
  };
}  // namespace radio

#endif  // WSNAMEUPDATED_HPP
