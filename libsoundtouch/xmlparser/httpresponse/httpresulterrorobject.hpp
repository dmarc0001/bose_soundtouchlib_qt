#ifndef RESULTERROROBJECT_HPP
#define RESULTERROROBJECT_HPP

#include <qglobal.h>
#include <QDomElement>
#include <QDomNode>
#include <QObject>
#include <QVector>
#include <QtDebug>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT HttpResultErrorObject : public IResponseObject
  {
    private:
    QVector< ResponseError > errors;  //! Liste von Fehlermeldungen

    public:
    explicit HttpResultErrorObject( QDomElement *domElem, QObject *parent );
    ~HttpResultErrorObject() override;
    //
    // GETTER
    //
    QVector< ResponseError > getErrors() const;

    private:
    void parseError( QDomNode *node );  //! Parse Fehlermeldungen
  };
}  // namespace bose_soundtoch_lib
#endif  // RESULTERROROBJECT_HPP
