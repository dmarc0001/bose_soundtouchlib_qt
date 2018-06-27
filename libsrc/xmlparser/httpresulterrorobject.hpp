#ifndef RESULTERROROBJECT_HPP
#define RESULTERROROBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include <QtDebug>
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class HttpResultErrorObject : public IResponseObject
  {
    private:
    QVector< ResponseError > errors;

    public:
    explicit HttpResultErrorObject( QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpResultErrorObject() override;
    // Getter Funktionen
    QVector< ResponseError > getErrors() const;

    private:
    void parseError( void );
  };
}  // namespace bose_soundtoch_lib
#endif  // RESULTERROROBJECT_HPP
