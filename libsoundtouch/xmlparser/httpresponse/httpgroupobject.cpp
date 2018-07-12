#include "httpgroupobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpGroupObject::HttpGroupObject
   * @param xmlreader
   * @param parent
   */
  HttpGroupObject::HttpGroupObject( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    QDomNodeList rootChildNodesList;
    //
    // Objekt für zwei Fälle als httpresponse und ws update
    //
    Q_ASSERT( domElem->tagName() == QLatin1String( "group" ) || domElem->tagName() == QLatin1String( "groupUpdated" ) );
    //
    // group http oder ws responde groupUpdated
    //
    if ( domElem->tagName() == QLatin1String( "groupUpdated" ) )
    {
      resultType = ResultobjectType::U_GROUP;
      rootChildNodesList = domElem->firstChild().childNodes();
      QDomNode rnode( domElem->firstChild() );
      groupId = IResponseObject::getAttribute( &rnode, QLatin1String( "id" ) );
    }
    else
    {
      if ( domElem->tagName() == QLatin1String( "group" ) )
      {
        resultType = ResultobjectType::R_GROUPS;
        rootChildNodesList = domElem->childNodes();
        groupId = IResponseObject::getAttribute( domElem, QLatin1String( "id" ) );
      }
      else
      {
        resultType = ResultobjectType::R_UNKNOWN;
        return;
      }
    }
    //
    // ID finden (Attribute von <group>)
    //
    qDebug() << "...";
    qDebug() << "groupid: " << groupId;
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    for ( int nodeIdx = 0; nodeIdx < rootChildNodesList.length(); nodeIdx++ )
    {
      QDomNode currNode( rootChildNodesList.item( nodeIdx ) );
      if ( currNode.isNull() )
        continue;
      //
      // unterscheide die Knoten
      // der Name ist hier als QString
      //
      QString currName( currNode.nodeName() );
      //
      if ( currName == QLatin1String( "name" ) )
      {
        //
        // Name der Gruppe
        //
        name = currNode.toElement().text();
        qDebug() << "group name is " << name;
      }
      else if ( currName == QLatin1String( "masterDeviceId" ) )
      {
        //
        // ID des Masters in der Gruppe
        //
        masterDeviceId = currNode.toElement().text();
        qDebug() << "master device is " << masterDeviceId;
      }
      else if ( currName == QLatin1String( "senderIpAddress" ) )
      {
        //
        // IP des Senders
        //
        _senderIpAddress = currNode.toElement().text();
        qDebug() << "sender ip is " << _senderIpAddress;
      }
      else if ( currName == QLatin1String( "status" ) )
      {
        //
        // Status der Gruppe
        //
        status = currNode.toElement().text();
        qDebug() << "group status is " << status;
      }
      else if ( currName == QLatin1String( "roles" ) )
      {
        //
        // beschreibt Produkt rolle in der Gruppe
        //
        parseRoles( &currNode );
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag: " << currName << " --> " << currNode.toElement().text();
      }
    }
  }

  HttpGroupObject::~HttpGroupObject()
  {
    qDebug() << "...";
  }

  void HttpGroupObject::parseRoles( QDomNode *node )
  {
    Q_ASSERT( node->nodeName() == QLatin1String( "roles" ) );
    //
    // Rollen des Gerätes in der Gruppe lesen
    //
    qDebug() << "...";
    //
    // lese soweit neue Elemente vorhanden sind
    //
    QDomNodeList childNodesList( node->childNodes() );
    for ( int nodeIdx = 0; nodeIdx < childNodesList.length(); nodeIdx++ )
    {
      QDomNode currNode( childNodesList.item( nodeIdx ) );
      if ( currNode.isNull() )
        continue;
      QString currName( currNode.nodeName() );
      if ( currName == QLatin1String( "groupRole" ) )
      {
        //
        // beschreibt eine Rolle des Produktes in der Gruppe
        //
        parseGroupRole( &currNode );
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag: " << currName << " --> " << currNode.toElement().text();
      }
    }
  }

  void HttpGroupObject::parseGroupRole( QDomNode *node )
  {
    Q_ASSERT( node->nodeName() == QLatin1String( "groupRole" ) );
    //
    // Rollen des Gerätes in der Gruppe lesen
    //
    qDebug() << "...";
    GroupRole role;
    //
    //
    // lese soweit neue Elemente vorhanden sind
    //
    QDomNodeList childNodesList( node->childNodes() );
    for ( int nodeIdx = 0; nodeIdx < childNodesList.length(); nodeIdx++ )
    {
      QDomNode currNode( childNodesList.item( nodeIdx ) );
      if ( currNode.isNull() )
        continue;
      QString currName( currNode.nodeName() );
      if ( currName == QLatin1String( "deviceId" ) )
      {
        //
        // Geräte ID dieser Rolle
        //
        role.deviceId = currNode.toElement().text();
        qDebug() << "grouprole deviceId is " << role.deviceId;
      }
      if ( currName == QLatin1String( "role" ) )
      {
        //
        // Rolle des Gerätes
        //
        role.role = currNode.toElement().text();
        qDebug() << "grouprole is " << role.role;
      }
      if ( currName == QLatin1String( "ipAddress" ) )
      {
        //
        // Adrese des Gerätes
        //
        role.ipAddress = currNode.toElement().text();
        qDebug() << "group device addr is " << role.ipAddress;
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag: " << currName << " --> " << currNode.toElement().text();
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
