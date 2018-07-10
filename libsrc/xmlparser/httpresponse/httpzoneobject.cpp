#include "xmlparser/httpresponse/httpzoneobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpZoneObject::HttpZoneObject
   * @param xmlreader
   * @param parent
   */
  HttpZoneObject::HttpZoneObject( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "zone" ) );
    resultType = ResultobjectType::R_ZONES;
    //
    // master finden (Attribute von <zone>)
    //
    qDebug() << "...";
    master = IResponseObject::getAttribute( reader, QLatin1String( "master" ) );
    qDebug() << "zone master: " << master;
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      //
      // das nächste element bearbeiten, welches ist es?
      //
      if ( reader->name() == QLatin1String( "member" ) )
      {
        //
        // ein Mitglied in der Zone
        //
        SoundTouchMemberObject member;
        member.first = IResponseObject::getAttribute( reader, QLatin1String( "ipaddress" ) );
        // lese MAC/ID des Slave/Master
        member.second = reader->readElementText();
        qDebug() << "member ID/MAC %1" << member.second;
        members.append( member );
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
