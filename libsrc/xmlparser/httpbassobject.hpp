#ifndef BASSOBJECT_HPP
#define BASSOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include "../logging/Logger.hpp"
#include "iresponseobject.hpp"

namespace radio
{
  class HttpBassObject : public IResponseObject
  {
    private:
    QString deviceId;
    int targetbass;
    int actualbass;

    public:
    HttpBassObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpBassObject() override;
    // getter Funktionen
    QString getDeviceId() const;
    int getTargetBass() const;
    int getActualBass() const;
  };
}  // namespace radio
#endif  // BASSOBJECT_HPP
