#include "wsnowselectionupdated.hpp"
#include <QDateTime>

namespace bose_soundtoch_lib
{
  WsNowSelectionUpdated::WsNowSelectionUpdated( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "nowSelectionUpdated" ) );
    int countContentItem = 0;
    resultType = ResultobjectType::U_SELECTION;
    qDebug() << "...";
    //
    //
    // lese soweit neue Elemente vorhanden sind
    //
    QDomNode currNode( domElem->firstChild() );
    if ( currNode.isNull() )
      return;
    //
    // unterscheide die Knoten
    // der Name ist hier als QString
    //
    QString currName( currNode.nodeName() );
    //
    if ( currName == QLatin1String( "preset" ) )
    {
      qDebug() << "found tag preset...";
      preset.id = IResponseObject::getAttribute( &currNode, QLatin1String( "id" ) );
      qDebug() << "preset id: " << preset.id;
      preset.createdOn = static_cast< qint64 >( IResponseObject::getAttribute( &currNode, QLatin1String( "createdOn" ) ).toLong() );
      qDebug() << "preset created: " << preset.createdOn << " ("
               << QDateTime::fromSecsSinceEpoch( preset.createdOn ).toString( "dd.MM.yyyy hh:mm:ss" ) << ")";
      preset.updatedOn = static_cast< qint64 >( IResponseObject::getAttribute( &currNode, QLatin1String( "updatedOn" ) ).toLong() );
      qDebug() << "preset updated: " << preset.updatedOn << " ("
               << QDateTime::fromSecsSinceEpoch( preset.updatedOn ).toString( "dd.MM.yyyy hh:mm:ss" ) << ")";
      //
      // jetzt alle childknoten von preset lesen (sollte nur ContentItem sein)
      //
      QDomNodeList childNodesList( currNode.childNodes() );
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

        if ( currName.compare( QLatin1String( "ContentItem" ), Qt::CaseInsensitive ) == 0 )
        {
          countContentItem++;
          if ( countContentItem > 1 )
          {
            qWarning() << "there is mor than one ContentItem in preset structure. This is an mistake!";
          }
          preset.contentItem = std::unique_ptr< ContentItem >( new ContentItem( &currNode, this ) );
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
    else
    {
      //
      // unsupportet elements
      //
      qWarning() << "unsupported tag: " << currName << " --> " << currNode.toElement().text();
    }
  }  // namespace bose_soundtoch_lib

  WsNowSelectionUpdated::~WsNowSelectionUpdated()
  {
    qDebug() << "...";
  }

  DevicePreset WsNowSelectionUpdated::getDevicePresets() const
  {
    return ( preset );
  }
}  // namespace bose_soundtoch_lib
