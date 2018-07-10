#include "xmlparser/httpresponse/httpgroupobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpGroupObject::HttpGroupObject
   * @param xmlreader
   * @param parent
   */
  HttpGroupObject::HttpGroupObject( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    //
    // Objekt für zwei Fälle als httpresponse und ws update
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
      // if ( reader -> readNextStartElement() && !reader->hasError() )
      while ( IResponseObject::getNextStartTag( reader ) )
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
    qDebug() << "...";
    groupId = IResponseObject::getAttribute( reader, QLatin1String( "id" ) );
    qDebug() << "groupid: " << groupId;
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( IResponseObject::getNextStartTag( reader ) )
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
        qDebug() << "group name is " << name;
      }
      else if ( reader->name() == QLatin1String( "masterDeviceId" ) )
      {
        //
        // ID des Masters in der Gruppe
        //
        masterDeviceId = reader->readElementText();
        qDebug() << "master device is " << masterDeviceId;
      }
      else if ( reader->name() == QLatin1String( "senderIpAddress" ) )
      {
        //
        // IP des Senders
        //
        _senderIpAddress = reader->readElementText();
        qDebug() << "sender ip is " << _senderIpAddress;
      }
      else if ( reader->name() == QLatin1String( "status" ) )
      {
        //
        // Status der Gruppe
        //
        status = reader->readElementText();
        qDebug() << "group status is " << status;
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
        qWarning() << "unsupported tag: " << reader->name().toString() << " --> " << reader->readElementText();
      }
    }
  }

  HttpGroupObject::~HttpGroupObject()
  {
    qDebug() << "...";
  }

  void HttpGroupObject::parseRoles( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "roles" ) );
    //
    // Rollen des Gerätes in der Gruppe lesen
    //
    qDebug() << "...";
    //
    while ( IResponseObject::getNextStartTag( reader ) )
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
        qWarning() << "unsupported tag: " << reader->name().toString() << " --> " << reader->readElementText();
      }
    }
  }

  void HttpGroupObject::parseGroupRole( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "groupRole" ) );
    //
    // Rollen des Gerätes in der Gruppe lesen
    //
    qDebug() << "...";
    GroupRole role;
    //
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      if ( reader->name() == QLatin1String( "deviceId" ) )
      {
        //
        // Geräte ID dieser Rolle
        //
        role.deviceId = reader->readElementText();
        qDebug() << "grouprole deviceId is " << role.deviceId;
      }
      if ( reader->name() == QLatin1String( "role" ) )
      {
        //
        // Rolle des Gerätes
        //
        role.role = reader->readElementText();
        qDebug() << "grouprole is " << role.role;
      }
      if ( reader->name() == QLatin1String( "ipAddress" ) )
      {
        //
        // Adrese des Gerätes
        //
        role.ipAddress = reader->readElementText();
        qDebug() << "group device addr is " << role.ipAddress;
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag: " << reader->name().toString() << " --> " << reader->readElementText();
      }
    }
    roles.append( role );
  }

  //
  // GETTER
  //

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

}  // namespace bose_soundtoch_lib
