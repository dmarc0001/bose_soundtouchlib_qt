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
    int targetbass;  //! gewünschte Einstellung
    int actualbass;  //! aktuelle Einstellung

    public:
    HttpBassObject( QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpBassObject() override;
    //
    // GETTER
    //
    int getTargetBass() const;
    int getActualBass() const;
  };
}  // namespace bose_soundtoch_lib
#endif  // BASSOBJECT_HPP
