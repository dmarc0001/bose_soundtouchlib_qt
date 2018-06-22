#ifndef BASSCAPABILITYS_HPP
#define BASSCAPABILITYS_HPP

#include <qglobal.h>
#include <QObject>
#include "../logging/Logger.hpp"
#include "iresponseobject.hpp"

namespace radio
{
  class BassCapabilities : public IResponseObject
  {
    private:
    QString deviceId;
    bool bassAvailable = false;
    int bassMin;
    int bassMax;
    int bassDefault;

    public:
    BassCapabilities( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~BassCapabilities();
  };
}  // namespace radio
#endif  // BASSCAPABILITYS_HPP
