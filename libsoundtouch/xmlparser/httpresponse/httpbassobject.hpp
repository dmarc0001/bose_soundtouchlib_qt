#ifndef BASSOBJECT_HPP
#define BASSOBJECT_HPP

#include <qglobal.h>
#include <QDomElement>
#include <QDomNode>
#include <QObject>
#include <QtDebug>
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT HttpBassObject : public IResponseObject
  {
    private:
    int targetbass;  //! gewünschte Einstellung
    int actualbass;  //! aktuelle Einstellung

    public:
    HttpBassObject( QDomElement *domElem, QObject *parent );
    ~HttpBassObject() override;
    //
    // GETTER
    //
    int getTargetBass() const;
    int getActualBass() const;
  };
}  // namespace bose_soundtoch_lib
#endif  // BASSOBJECT_HPP
