#ifndef BASSOBJECT_HPP
#define BASSOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QtDebug>
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class HttpBassObject : public IResponseObject
  {
    private:
    int targetbass;
    int actualbass;

    public:
    HttpBassObject( QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpBassObject() override;
    // getter Funktionen
    int getTargetBass() const;
    int getActualBass() const;
  };
}  // namespace bose_soundtoch_lib
#endif  // BASSOBJECT_HPP
