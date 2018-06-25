#ifndef GROUPOBJECT_HPP
#define GROUPOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include "../logging/Logger.hpp"
#include "soundtouch_response.hpp"
#include "iresponseobject.hpp"

namespace radio
{
  //! class with SoundTouch 10 only important
  class GroupObject : public IResponseObject
  {
    private:
    QString groupId;
    QString name;
    QString masterDeviceId;
    QVector< GroupRole > roles;
    QString _senderIpAddress;  //! unsupported
    QString status;

    public:
    explicit GroupObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~GroupObject() override;
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
}  // namespace radio
#endif  // GROUPOBJECT_HPP
