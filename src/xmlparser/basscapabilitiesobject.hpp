#ifndef BASSCAPABILITYS_HPP
#define BASSCAPABILITYS_HPP

#include <qglobal.h>
#include <QObject>
#include "../logging/Logger.hpp"
#include "iresponseobject.hpp"

namespace radio
{
  class BassCapabilitiesObject : public IResponseObject
  {
    private:
    QString deviceId;
    bool bassAvailable = false;
    int bassMin;
    int bassMax;
    int bassDefault;

    public:
    BassCapabilitiesObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~BassCapabilitiesObject();
  };
}  // namespace radio
#endif  // BASSCAPABILITYS_HPP
