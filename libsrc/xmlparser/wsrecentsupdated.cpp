#include "wsrecentsupdated.hpp"

namespace bose_soundtoch_lib
{
  WsRecentsUpdated::WsRecentsUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "recentsUpdated" ) );
    resultType = ResultobjectType::U_RECENTS_UNSUPPORTED;
    qDebug() << "(unsupported)...";
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
            qWarning() << "recentsUpdated->recents: expected: recent, found: " << reader->name().toString();
          }
        }
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag:" << reader->name().toString() << " --> " << reader->readElementText();
      }
    }
    while ( reader->readNextStartElement() && !reader->hasError() )
      ;
  }

  WsRecentsUpdated::~WsRecentsUpdated()
  {
    qDebug() << "...";
  }

  void WsRecentsUpdated::readRecent( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "recent" ) );
    qDebug() << "...";
    DeviceRecent recent;
    recent.deviceId = getAttibute( reader, QLatin1String( "deviceId" ) );
    recent.utcTime = static_cast< qint64 >( getAttibute( reader, QLatin1String( "utcTime" ) ).toLong() );
    recent.id = static_cast< qint64 >( getAttibute( reader, QLatin1String( "id" ) ).toLong() );
    if ( reader->readNextStartElement() && !reader->hasError() )
    {
      if ( reader->name() == QLatin1String( "contentItem" ) )
      {
        recent.contentItem = std::shared_ptr< ContentItem >( new ContentItem( reader, this ) );
      }
      else
      {
        qWarning() << "expected item <contentItem> read: " << reader->name().toString();
      }
    }
    recents.append( recent );
  }

}  // namespace bose_soundtoch_lib
