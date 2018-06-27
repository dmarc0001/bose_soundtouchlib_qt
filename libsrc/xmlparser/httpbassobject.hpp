#ifndef BASSOBJECT_HPP
#define BASSOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include "../logging/Logger.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class HttpBassObject : public IResponseObject
  {
    private:
    int targetbass;
    int actualbass;

    public:
    HttpBassObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpBassObject() override;
    // getter Funktionen
    int getTargetBass() const;
    int getActualBass() const;
  };
}  // namespace radio
#endif  // BASSOBJECT_HPP
