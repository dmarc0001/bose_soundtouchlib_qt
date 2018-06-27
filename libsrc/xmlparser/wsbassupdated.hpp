#ifndef WSBASSUPDATED_HPP
#define WSBASSUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsBassUpdated : public IResponseObject
  {
    private:
    bool updatet;

    public:
    explicit WsBassUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsBassUpdated() override;
  };
}  // namespace radio

#endif  // WSBASSUPDATED_HPP
