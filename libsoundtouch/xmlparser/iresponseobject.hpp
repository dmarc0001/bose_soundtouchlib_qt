#ifndef IRESPONSEOBJECT_HPP
#define IRESPONSEOBJECT_HPP

#include <qglobal.h>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QMap>
#include <QObject>
#include <QRegExp>
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
    static const QRegExp shortTags;                            //! erkennt verkürzte tags als Starttag

    protected:
    QDomElement *domElem;         //! für vererbte Objekte das DOM Element
    ResultobjectType resultType;  //! für vererbte Objekte dar Typ dieses Objektes
    QString deviceId;             //! die Device id, bei (fast) jedem objekt

    public:
    explicit IResponseObject( QDomElement *_domElem, QObject *parent );
    virtual ~IResponseObject();
    //! Hilfsfunktion zum Lesen der Attribute eines TAG
    static QString getAttribute( QDomNode *domNode, QLatin1String name );
    static QString getAttribute( QDomElement *domElement, QLatin1String name );
    //
    // GETTER
    //
    ResultobjectType getResultType( void );
    void setDeviceId( const QString &devId );
    QString getResultTypeName( void ) const;
    QString getDeviceId() const;
  };
}  // namespace bose_soundtoch_lib
#endif  // IRESPONSEOBJECT_HPP
