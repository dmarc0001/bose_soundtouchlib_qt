#ifndef ZONEOBJECT_HPP
#define ZONEOBJECT_HPP

#include <qglobal.h>
#include <QDomElement>
#include <QDomNode>
#include <QObject>
#include <QtDebug>
#include <memory>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT HttpZoneObject : public IResponseObject
  {
    private:
    QString master;                //! Zonenmaster
    SoundTouchMemberList members;  //! liste von mitgliedern

    public:
    explicit HttpZoneObject( QDomElement *domElem, QObject *parent );
    ~HttpZoneObject() override;
    //
    // Getter Funktionen
    //
    QString getMaster() const;
    SoundTouchMemberList getMembers() const;
  };
}  // namespace bose_soundtoch_lib
#endif  // ZONEOBJECT_HPP
