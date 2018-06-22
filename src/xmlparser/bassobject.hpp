#ifndef BASSOBJECT_HPP
#define BASSOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include "../logging/Logger.hpp"
#include "iresponseobject.hpp"

namespace radio
{
  class BassObject : public IResponseObject
  {
    private:
    QString deviceId;
    int targetbass;
    int actualbass;

    public:
    BassObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~BassObject() override;
    // getter Funktionen
    QString getDeviceId() const;
    int getTargetBass() const;
    int getActualBass() const;
  };
}  // namespace radio
#endif  // BASSOBJECT_HPP
