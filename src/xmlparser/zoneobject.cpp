#include "zoneobject.hpp"

namespace radio
{
  ZoneObject::ZoneObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "zone" ) );
    resultType = ResultobjectType::R_ZONES;
    //
    // master finden (Attribute von <zone>)
    //
    lg->debug( "ZoneObject::ZoneObject..." );
    lg->debug( "ZoneObject::ZoneObject: check for attribute in \"zone\"..." );
    QXmlStreamAttributes attr = reader->attributes();
    if ( attr.hasAttribute( QLatin1String( "master" ) ) )
    {
      master = attr.value( QLatin1String( "master" ) ).toString();
      lg->debug( QString( "ZoneObject::ZoneObject: attribute \"master\" has value %1" ).arg( master ) );
    }
    else
    {
      lg->warn( "ZoneObject::ZoneObject: there is no attribute \"master\"..." );
    }
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
        QXmlStreamAttributes attr = reader->attributes();
        if ( attr.hasAttribute( QLatin1String( "ipaddress" ) ) )
        {
          // lese IP Adresse des Member
          member.first = attr.value( QLatin1String( "ipaddress" ) ).toString();
          lg->debug( QString( "ZoneObject::ZoneObject: attribute \"ipaddress\" has value %1" ).arg( member.first ) );
        }
        else
        {
          lg->warn( "ZoneObject::ZoneObject: there is no attribute \"ipaddress\"..." );
        }
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

  ZoneObject::~ZoneObject()
  {
  }
}  // namespace radio
