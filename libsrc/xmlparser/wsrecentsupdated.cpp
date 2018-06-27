#include "wsrecentsupdated.hpp"

namespace bose_soundtoch_lib
{
  WsRecentsUpdated::WsRecentsUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "recentsUpdated" ) );
    resultType = ResultobjectType::U_RECENTS_UNSUPPORTED;
    lg->debug( "WsRecentsUpdate::WsRecentsUpdate (unsupported)..." );
    if ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
      if ( reader->name() == QLatin1String( "recents" ) )
      {
        while ( reader->readNextStartElement() && !reader->hasError() )
        {
          if ( reader->name() == QLatin1String( "recent" ) )
          {
            readRecent();
          }
          else
          {
            lg->warn( QString( "WsRecentsUpdated::WsRecentsUpdated: recentsUpdated->recents: found: %1, expected: recent" )
                          .arg( reader->name().toString() ) );
          }
        }
      }
      else
      {
        //
        // unsupportet elements
        //
        lg->debug( QString( "WsRecentsUpdated::WsRecentsUpdated: <%1> -> \"%2\"" )
                       .arg( reader->name().toString() )
                       .arg( reader->readElementText() ) );
      }
    }
    while ( reader->readNextStartElement() && !reader->hasError() )
      ;
  }

  WsRecentsUpdated::~WsRecentsUpdated()
  {
    lg->debug( "WsRecentsUpdate::~WsRecentsUpdate..." );
  }

  void WsRecentsUpdated::readRecent( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "recent" ) );
    DeviceRecent recent;
    recent.deviceId = getAttibute( reader, QLatin1String( "deviceId" ) );
    recent.utcTime = static_cast< qint64 >( getAttibute( reader, QLatin1String( "utcTime" ) ).toLong() );
    recent.id = static_cast< qint64 >( getAttibute( reader, QLatin1String( "id" ) ).toLong() );
    if ( reader->readNextStartElement() && !reader->hasError() )
    {
      if ( reader->name() == QLatin1String( "contentItem" ) )
      {
        recent.contentItem = std::shared_ptr< ContentItem >( new ContentItem( lg, reader, this ) );
      }
      else
      {
        lg->warn( QString( "WsRecentsUpdated::readRecent: expected item <contentItem> read: %1" ).arg( reader->name().toString() ) );
      }
    }
    recents.append( recent );
  }

}  // namespace bose_soundtoch_lib
