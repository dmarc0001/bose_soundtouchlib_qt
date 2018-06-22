#ifndef GROUPOBJECT_HPP
#define GROUPOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include "../global/bsoundtouch_global.hpp"
#include "../logging/Logger.hpp"
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

    private:
    void parseRoles( void );
    void parseGroupRole( void );
  };
}  // namespace radio
#endif  // GROUPOBJECT_HPP
