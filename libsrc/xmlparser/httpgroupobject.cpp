#include "httpgroupobject.hpp"

namespace bose_soundtoch_lib
{
  HttpGroupObject::HttpGroupObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    //
    // Objekt für zwei Fälle
    //
    Q_ASSERT( reader->isStartElement() &&
              ( reader->name() == QLatin1String( "group" ) || reader->name() == QLatin1String( "groupUpdated" ) ) );
    if ( reader->name() == QLatin1String( "group" ) )
    {
      // response für getGroup
      resultType = ResultobjectType::R_GROUPS;
    }
    else
    {
      if ( reader->readNextStartElement() && !reader->hasError() )
      {
        if ( reader->name() == QLatin1String( "groupUpdated" ) )
        {
          // callback für updatedGroup
          resultType = ResultobjectType::U_GROUP;
        }
        else
        {
          resultType = ResultobjectType::R_UNKNOWN;
          return;
        }
      }
    }
    //
    // ID finden (Attribute von <group>)
    //
    lg->debug( "GroupObject::GroupObject..." );
    lg->debug( "GroupObject::GroupObject: check for attribute in \"group\"..." );
    groupId = getAttibute( reader, QLatin1String( "id" ) );
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      //
      // das nächste element bearbeiten, welches ist es?
      //
      if ( reader->name() == QLatin1String( "name" ) )
      {
        //
        // Name der Gruppe
        //
        name = reader->readElementText();
        lg->debug( QString( "GroupObject::GroupObject: group name is %1" ).arg( name ) );
      }
      else if ( reader->name() == QLatin1String( "masterDeviceId" ) )
      {
        //
        // ID des Masters in der Gruppe
        //
        masterDeviceId = reader->readElementText();
        lg->debug( QString( "GroupObject::GroupObject: master device is %1" ).arg( masterDeviceId ) );
      }
      else if ( reader->name() == QLatin1String( "senderIpAddress" ) )
      {
        //
        // IP des Senders
        //
        _senderIpAddress = reader->readElementText();
        lg->debug( QString( "GroupObject::GroupObject: sender ip is %1" ).arg( _senderIpAddress ) );
      }
      else if ( reader->name() == QLatin1String( "status" ) )
      {
        //
        // Status der Gruppe
        //
        status = reader->readElementText();
        lg->debug( QString( "GroupObject::GroupObject: group status is %1" ).arg( status ) );
      }
      else if ( reader->name() == QLatin1String( "roles" ) )
      {
        //
        // beschreibt Produkt rolle in der Gruppe
        //
        parseRoles();
      }
      else
      {
        //
        // unsupportet elements
        //
        lg->debug( QString( "GroupObject::GroupObject: %1 -> %2" ).arg( reader->name().toString() ).arg( reader->readElementText() ) );
      }
    }
  }

  HttpGroupObject::~HttpGroupObject()
  {
    lg->debug( "GroupObject::~GroupObject()..." );
  }

  void HttpGroupObject::parseRoles( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "roles" ) );
    //
    // Rollen des Gerätes in der Gruppe lesen
    //
    lg->debug( "GroupObject::parseRoles..." );
    //
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      if ( reader->name() == QLatin1String( "groupRole" ) )
      {
        //
        // beschreibt eine Rolle des Produktes in der Gruppe
        //
        parseGroupRole();
      }
      else
      {
        //
        // unsupportet elements
        //
        lg->debug( QString( "GroupObject::GroupObject: %1 -> %2" ).arg( reader->name().toString() ).arg( reader->readElementText() ) );
      }
    }
  }

  void HttpGroupObject::parseGroupRole( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "groupRole" ) );
    //
    // Rollen des Gerätes in der Gruppe lesen
    //
    lg->debug( "GroupObject::parseGroupRole..." );
    GroupRole role;
    //
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      if ( reader->name() == QLatin1String( "deviceId" ) )
      {
        //
        // Geräte ID dieser Rolle
        //
        role.deviceId = reader->readElementText();
        lg->debug( QString( "GroupObject::GroupObject: grouprole deviceId is %1" ).arg( role.deviceId ) );
      }
      if ( reader->name() == QLatin1String( "role" ) )
      {
        //
        // Rolle des Gerätes
        //
        role.role = reader->readElementText();
        lg->debug( QString( "GroupObject::GroupObject: grouprole is %1" ).arg( role.role ) );
      }
      if ( reader->name() == QLatin1String( "ipAddress" ) )
      {
        //
        // Adrese des Gerätes
        //
        role.ipAddress = reader->readElementText();
        lg->debug( QString( "GroupObject::GroupObject: group device addr is %1" ).arg( role.ipAddress ) );
      }
      else
      {
        //
        // unsupportet elements
        //
        lg->debug(
            QString( "GroupObject::parseGroupRole: %1 -> %2" ).arg( reader->name().toString() ).arg( reader->readElementText() ) );
      }
    }
    roles.append( role );
  }

  QString HttpGroupObject::getGroupId() const
  {
    return groupId;
  }

  QString HttpGroupObject::getName() const
  {
    return name;
  }

  QString HttpGroupObject::getMasterDeviceId() const
  {
    return masterDeviceId;
  }

  QString HttpGroupObject::getSenderIpAddress() const
  {
    return _senderIpAddress;
  }

  QString HttpGroupObject::getStatus() const
  {
    return status;
  }

}  // namespace radio
