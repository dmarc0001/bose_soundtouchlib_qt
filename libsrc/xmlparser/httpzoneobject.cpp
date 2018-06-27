#include "httpzoneobject.hpp"

namespace bose_soundtoch_lib
{
  HttpZoneObject::HttpZoneObject( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "zone" ) );
    resultType = ResultobjectType::R_ZONES;
    //
    // master finden (Attribute von <zone>)
    //
    qDebug() << "...";
    master = getAttibute( reader, QLatin1String( "master" ) );
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( reader->readNextStartElement() && !reader->hasError() )
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
        member.first = getAttibute( reader, QLatin1String( "ipaddress" ) );
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

  HttpZoneObject::~HttpZoneObject()
  {
    qDebug() << "...";
  }

  QString HttpZoneObject::getMaster() const
  {
    return master;
  }

  SoundTouchMemberList HttpZoneObject::getMembers() const
  {
    return members;
  }

}  // namespace bose_soundtoch_lib
