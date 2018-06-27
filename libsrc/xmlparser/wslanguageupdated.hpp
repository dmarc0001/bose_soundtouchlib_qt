#ifndef WSLANGUAGEUPDATED_HPP
#define WSLANGUAGEUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsLanguageUpdated : public IResponseObject
  {
    private:
    QString language;

    public:
    explicit WsLanguageUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsLanguageUpdated() override;
  };
}  // namespace radio

#endif  // WSLANGUAGEUPDATED_HPP
