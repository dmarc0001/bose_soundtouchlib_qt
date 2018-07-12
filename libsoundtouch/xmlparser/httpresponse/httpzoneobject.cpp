#include "httpzoneobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpZoneObject::HttpZoneObject
   * @param xmlreader
   * @param parent
   */
  HttpZoneObject::HttpZoneObject( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "zone" ) );
    resultType = ResultobjectType::R_ZONES;
    //
    // Device ID finden (Attribute von <info>)
    //
    qDebug() << "...";
    master = IResponseObject::getAttribute( domElem, QLatin1String( "master" ) );
    qDebug() << "zone master: " << master;
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    QDomNodeList rootChildNodesList( domElem->childNodes() );
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
      if ( currName == QLatin1String( "member" ) )
      {
        //
        // ein Mitglied in der Zone
        //
        SoundTouchMemberObject member;
        member.first = IResponseObject::getAttribute( &currNode, QLatin1String( "ipaddress" ) );
        // lese MAC/ID des Slave/Master
        member.second = currNode.toElement().text();
        qDebug() << "member ID/MAC " << member.second;
        members.append( member );
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

  /**
   * @brief HttpZoneObject::~HttpZoneObject
   */
  HttpZoneObject::~HttpZoneObject()
  {
    qDebug() << "...";
  }

  //
  // GETTER
  //

  QString HttpZoneObject::getMaster() const
  {
    return master;
  }

  SoundTouchMemberList HttpZoneObject::getMembers() const
  {
    return members;
  }

}  // namespace bose_soundtoch_lib
