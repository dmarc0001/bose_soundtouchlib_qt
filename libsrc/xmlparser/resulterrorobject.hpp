#ifndef RESULTERROROBJECT_HPP
#define RESULTERROROBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include "../logging/Logger.hpp"
#include "soundtouch_response.hpp"
#include "iresponseobject.hpp"

namespace radio
{
  class ResultErrorObject : public IResponseObject
  {
    private:
    QString deviceId;
    QVector< ResponseError > errors;

    public:
    explicit ResultErrorObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~ResultErrorObject() override;
    // Getter Funktionen
    QString getDeviceId() const;
    QVector< ResponseError > getErrors() const;

    private:
    void parseError( void );
  };
}  // namespace radio
#endif  // RESULTERROROBJECT_HPP
