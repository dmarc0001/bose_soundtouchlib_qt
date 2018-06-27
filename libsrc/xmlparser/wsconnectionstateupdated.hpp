#ifndef WSCONNECTIONSTATEUPDATED_HPP
#define WSCONNECTIONSTATEUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsConnectionStateUpdated : public IResponseObject
  {
    private:
    bool isUp;
    QString state;
    QString signal;

    public:
    explicit WsConnectionStateUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsConnectionStateUpdated() override;
  };
}  // namespace radio

#endif  // WSCONNECTIONSTATEUPDATED_HPP
