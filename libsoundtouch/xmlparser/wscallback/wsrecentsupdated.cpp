#include "wsrecentsupdated.hpp"
#include <QDateTime>

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
    if ( IResponseObject::getNextStartTag( reader ) )
    {
      // elemente zuende lesen und ignorieren
      if ( reader->name() == QLatin1String( "recents" ) )
      {
        while ( IResponseObject::getNextStartTag( reader ) )
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
    while ( IResponseObject::getNextStartTag( reader ) )
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
    recent.deviceId = IResponseObject::getAttribute( reader, QLatin1String( "deviceID" ) );
    qDebug() << "recent device id: " << recent.deviceId;
    recent.utcTime = static_cast< qint64 >( IResponseObject::getAttribute( reader, QLatin1String( "utcTime" ) ).toLong() );
    qDebug() << "recent time: " << recent.utcTime << " ("
             << QDateTime::fromSecsSinceEpoch( recent.utcTime ).toString( "dd.MM.yyyy hh:mm:ss" ) << ")";
    recent.id = static_cast< qint64 >( IResponseObject::getAttribute( reader, QLatin1String( "id" ) ).toLong() );
    qDebug() << "recent id: " << recent.id;
    if ( IResponseObject::getNextStartTag( reader ) )
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
