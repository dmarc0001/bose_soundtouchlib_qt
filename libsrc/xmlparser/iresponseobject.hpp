#ifndef IRESPONSEOBJECT_HPP
#define IRESPONSEOBJECT_HPP

#include <qglobal.h>
#include <QMap>
#include <QObject>
#include <QXmlStreamReader>
#include <QtDebug>
#include <memory>
#include "bsoundtouch_global.hpp"

namespace bose_soundtoch_lib
{
  class IResponseObject : public QObject
  {
    Q_OBJECT
    private:
    static const QMap< ResultobjectType, QString > typeNames;

    protected:
    QXmlStreamReader *reader;
    ResultobjectType resultType;
    QString deviceId;

    public:
    explicit IResponseObject( QXmlStreamReader *xmlreader, QObject *parent );
    virtual ~IResponseObject();
    ResultobjectType getResultType( void );
    void setDeviceId( QString &devId );
    QString getResultTypeName( void ) const;
    static QString getAttibute( QXmlStreamReader *reader, QLatin1String name );
    QString getDeviceId() const;
  };
}  // namespace bose_soundtoch_lib
#endif  // IRESPONSEOBJECT_HPP
