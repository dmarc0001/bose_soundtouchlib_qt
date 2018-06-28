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
    static const QMap< ResultobjectType, QString > typeNames;  //! convertiert type zu string

    protected:
    QXmlStreamReader *reader;     //! für vererbte Objekte der xml parser
    ResultobjectType resultType;  //! für vererbte Objekte dar Typ dieses Objektes
    QString deviceId;             //! die Device id, bei (fast) jedem objekt

    public:
    explicit IResponseObject( QXmlStreamReader *xmlreader, QObject *parent );
    virtual ~IResponseObject();
    //! Hilfsfunktion zum Lesen der Attribute eines TAG
    static QString getAttribute( QXmlStreamReader *reader, QLatin1String name );
    //
    // GETTER
    //
    ResultobjectType getResultType( void );
    void setDeviceId( QString &devId );
    QString getResultTypeName( void ) const;
    QString getDeviceId() const;
  };
}  // namespace bose_soundtoch_lib
#endif  // IRESPONSEOBJECT_HPP
