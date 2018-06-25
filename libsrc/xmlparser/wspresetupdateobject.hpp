#ifndef WSPRESETUPDATEOBJECT_HPP
#define WSPRESETUPDATEOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <memory>
#include "../logging/Logger.hpp"
#include "iresponseobject.hpp"
#include "soundtouch_response.hpp"

namespace radio
{
  class WsPresetUpdateObject : public IResponseObject
  {
    public:
    explicit WsPresetUpdateObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsPresetUpdateObject() override;
  };
}  // namespace radio
#endif  // WSPRESETUPDATEOBJECT_HPP
