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
    QVector< ResponseError > errors;  //! Liste von Fehlermeldungen

    public:
    explicit HttpResultErrorObject( QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpResultErrorObject() override;
    //
    // GETTER
    //
    QVector< ResponseError > getErrors() const;

    private:
    void parseError( void );  //! Parse Fehlermeldungen
  };
}  // namespace bose_soundtoch_lib
#endif  // RESULTERROROBJECT_HPP
