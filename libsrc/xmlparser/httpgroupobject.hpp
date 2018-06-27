#ifndef GROUPOBJECT_HPP
#define GROUPOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include <QtDebug>
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  //! class with SoundTouch 10 only important
  class HttpGroupObject : public IResponseObject
  {
    private:
    QString groupId;
    QString name;
    QString masterDeviceId;
    QVector< GroupRole > roles;
    QString _senderIpAddress;  //! unsupported
    QString status;

    public:
    explicit HttpGroupObject( QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpGroupObject() override;
    // Getter Funktionen
    QString getGroupId() const;
    QString getName() const;
    QString getMasterDeviceId() const;
    QString getSenderIpAddress() const;
    QString getStatus() const;

    private:
    void parseRoles( void );
    void parseGroupRole( void );
  };
}  // namespace bose_soundtoch_lib
#endif  // GROUPOBJECT_HPP
