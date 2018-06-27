#include "httpzoneobject.hpp"

namespace bose_soundtoch_lib
{
  HttpZoneObject::HttpZoneObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "zone" ) );
    resultType = ResultobjectType::R_ZONES;
    //
    // master finden (Attribute von <zone>)
    //
    lg->debug( "ZoneObject::ZoneObject..." );
    lg->debug( "ZoneObject::ZoneObject: check for attribute in \"zone\"..." );
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
        lg->debug( QString( "ZoneObject::ZoneObject: member ID/MAC %1" ).arg( member.second ) );
        members.append( member );
      }
      else
      {
        //
        // unsupportet elements
        //
        lg->debug( QString( "ZoneObject::ZoneObject: %1 -> %2" ).arg( reader->name().toString() ).arg( reader->readElementText() ) );
      }
    }
  }

  HttpZoneObject::~HttpZoneObject()
  {
    lg->debug( "ZoneObject::~ZoneObject..." );
  }

  QString HttpZoneObject::getMaster() const
  {
    return master;
  }

  SoundTouchMemberList HttpZoneObject::getMembers() const
  {
    return members;
  }

}  // namespace radio
