#ifndef RESULTERROROBJECT_HPP
#define RESULTERROROBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class HttpResultErrorObject : public IResponseObject
  {
    private:
    QVector< ResponseError > errors;

    public:
    explicit HttpResultErrorObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpResultErrorObject() override;
    // Getter Funktionen
    QVector< ResponseError > getErrors() const;

    private:
    void parseError( void );
  };
}  // namespace radio
#endif  // RESULTERROROBJECT_HPP
