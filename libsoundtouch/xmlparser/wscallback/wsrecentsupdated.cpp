#include "wsrecentsupdated.hpp"
#include <QDateTime>

namespace bose_soundtoch_lib
{
  /**
   * @brief WsRecentsUpdated::WsRecentsUpdated
   * @param xmlreader
   * @param parent
   */
  WsRecentsUpdated::WsRecentsUpdated( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "recentsUpdated" ) );
    resultType = ResultobjectType::U_RECENTS_UNSUPPORTED;
    qDebug() << "(unsupported)...";
    QDomNode recentElem( domElem->firstChild() );
    if ( !recentElem.isNull() && recentElem.nodeName() == QLatin1String( "recents" ) )
    {
      QDomNodeList childNodesList( recentElem.childNodes() );
      for ( int nodeIdx = 0; nodeIdx < childNodesList.length(); nodeIdx++ )
      {
        QDomNode currNode( childNodesList.item( nodeIdx ) );
        if ( currNode.isNull() )
          continue;
        //
        // unterscheide die Knoten
        // der Name ist hier als QString
        //
        QString currName( currNode.nodeName() );
        //
        if ( currName == QLatin1String( "recent" ) )
        {
          readRecent( &currNode );
        }
        else
        {
          qWarning() << "unsupported tag: " << currName;
        }
      }
    }
    else
    {
      //
      // unsupportet elements
      //
      qWarning() << "unsupported tag";
    }
  }

  /**
   * @brief WsRecentsUpdated::~WsRecentsUpdated
   */
  WsRecentsUpdated::~WsRecentsUpdated()
  {
    qDebug() << "...";
  }

  void WsRecentsUpdated::readRecent( QDomNode *node )
  {
    int contentItemCount = 0;
    Q_ASSERT( node->nodeName() == QLatin1String( "recent" ) );
    qDebug() << "...";
    DeviceRecent recent;
    recent.deviceId = IResponseObject::getAttribute( node, QLatin1String( "deviceID" ) );
    qDebug() << "recent device id: " << recent.deviceId;
    recent.utcTime = static_cast< qint64 >( IResponseObject::getAttribute( node, QLatin1String( "utcTime" ) ).toLong() );
    qDebug() << "recent time: " << recent.utcTime << " ("
             << QDateTime::fromSecsSinceEpoch( recent.utcTime ).toString( "dd.MM.yyyy hh:mm:ss" ) << ")";
    recent.id = static_cast< qint64 >( IResponseObject::getAttribute( node, QLatin1String( "id" ) ).toLong() );
    qDebug() << "recent id: " << recent.id;
    //
    // erzeuge Liste mit Kindknoten (sollte hier eigetnlich nur einer sein)
    //
    QDomNodeList childNodesList( node->childNodes() );
    for ( int nodeIdx = 0; nodeIdx < childNodesList.length(); nodeIdx++ )
    {
      QDomNode currNode( childNodesList.item( nodeIdx ) );
      if ( currNode.isNull() )
        continue;
      //
      // unterscheide die Knoten
      // der Name ist hier als QString
      //
      QString currName( currNode.nodeName() );
      if ( currName.compare( QLatin1String( "ContentItem" ), Qt::CaseInsensitive ) == 0 )
      {
        contentItemCount++;
        if ( contentItemCount > 1 )
        {
          qWarning() << "Element ContentItem in device preset mor than one times in preset structure. This is an mistake....";
        }
        recent.contentItem = std::unique_ptr< ContentItem >( new ContentItem( &currNode, this ) );
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag (expected ContentItem): " << currName << " --> " << currNode.toElement().text();
      }
    }
    qDebug() << "add recent entry to list...";
    recents.append( recent );
    qDebug() << "now " << recents.count() << " entry...";
  }

}  // namespace bose_soundtoch_lib
