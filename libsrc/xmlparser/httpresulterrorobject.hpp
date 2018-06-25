#ifndef RESULTERROROBJECT_HPP
#define RESULTERROROBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include "../logging/Logger.hpp"
#include "iresponseobject.hpp"
#include "soundtouch_response.hpp"

namespace radio
{
  class HttpResultErrorObject : public IResponseObject
  {
    private:
    QString deviceId;
    QVector< ResponseError > errors;

    public:
    explicit HttpResultErrorObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpResultErrorObject() override;
    // Getter Funktionen
    QString getDeviceId() const;
    QVector< ResponseError > getErrors() const;

    private:
    void parseError( void );
  };
}  // namespace radio
#endif  // RESULTERROROBJECT_HPP
