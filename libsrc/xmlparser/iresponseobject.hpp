#ifndef IRESPONSEOBJECT_HPP
#define IRESPONSEOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QXmlStreamReader>
#include <memory>
#include "../logging/Logger.hpp"
#include "soundtouch_response.hpp"

namespace radio
{
  class IResponseObject : public QObject
  {
    Q_OBJECT
    protected:
    std::shared_ptr< Logger > lg;
    QXmlStreamReader *reader;
    ResultobjectType resultType;

    public:
    static const char *responseTypeNames[];

    public:
    explicit IResponseObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    virtual ~IResponseObject();
    ResultobjectType getResultType( void );
    QString getResultTypeName( void ) const;
  };
}  // namespace radio
#endif  // IRESPONSEOBJECT_HPP
