#ifndef GROUPOBJECT_HPP
#define GROUPOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include <QtDebug>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  //! class with SoundTouch 10 only important
  class HttpGroupObject : public IResponseObject
  {
    private:
    QString groupId;             //! Gruppen id
    QString name;                //! Name der Gruppe
    QString masterDeviceId;      //! Master der Gruppe
    QVector< GroupRole > roles;  //! Liste mit Rollen der Geräte
    QString _senderIpAddress;    //! unsupported
    QString status;              //! Status der Gruppe

    public:
    explicit HttpGroupObject( QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpGroupObject() override;
    //
    // GETTER
    //
    QString getGroupId() const;
    QString getName() const;
    QString getMasterDeviceId() const;
    QString getSenderIpAddress() const;
    QString getStatus() const;

    private:
    void parseRoles( void );      //! Rollen parsen
    void parseGroupRole( void );  //! Gruppenrolle parsen
  };
}  // namespace bose_soundtoch_lib
#endif  // GROUPOBJECT_HPP
