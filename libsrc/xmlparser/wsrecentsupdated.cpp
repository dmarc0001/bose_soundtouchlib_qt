#include "wsrecentsupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsRecentsUpdated::WsRecentsUpdated
   * @param xmlreader
   * @param parent
   */
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
    //
    // eventuelle Reste zuende lesen und ignorieren
    //
    while ( reader->readNextStartElement() && !reader->hasError() )
      ;
  }

  /**
   * @brief WsRecentsUpdated::~WsRecentsUpdated
   */
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
        qDebug() << "contentItem found.";
        recent.contentItem = std::shared_ptr< ContentItem >( new ContentItem( reader, this ) );
      }
      else
      {
        qWarning() << "expected item <contentItem> read: " << reader->name().toString();
      }
    }
    qDebug() << "add recent entry to list...";
    recents.append( recent );
    qDebug() << "now " << recents.count() << " entry...";
  }

}  // namespace bose_soundtoch_lib
