#include "httpsourcesobject.hpp"

namespace bose_soundtoch_lib
{
  HttpSourcesObject::HttpSourcesObject( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "sources" ) );
    resultType = ResultobjectType::R_DEVICE_INFO;
    //
    // Device ID finden (Attribute von <info>)
    //
    qDebug() << "...";
    deviceId = IResponseObject::getAttribute( domElem, QLatin1String( "deviceID" ) );
    qDebug() << "device id: " << deviceId;
    //
    // lese soweit neue Elemente vorhanden sind
    //
    QDomNodeList rootChildNodesList( domElem->childNodes() );
    for ( int nodeIdx = 0; nodeIdx < rootChildNodesList.length(); nodeIdx++ )
    {
      QDomNode currNode( rootChildNodesList.item( nodeIdx ) );
      if ( currNode.isNull() )
        continue;
      //
      // unterscheide die Knoten
      // der Name ist hier als QString
      //
      QString currName( currNode.nodeName() );
      //
      if ( currName == QLatin1String( "sourceItem" ) )
      {
        //
        // Ein Eintrag über eine source
        //
        parseSourceItem( &currNode );
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

  HttpSourcesObject::~HttpSourcesObject()
  {
    qDebug() << "...";
  }

  void HttpSourcesObject::parseSourceItem( QDomNode *node )
  {
    Q_ASSERT( node->nodeName() == QLatin1String( "sourceItem" ) );
    SourceItem sourceItem;
    //
    // source infos finden (Attribute von <sourceItem>)
    //
    qDebug() << "...";
    sourceItem.source = IResponseObject::getAttribute( node, QLatin1String( "source" ) );
    qDebug() << "source: " << sourceItem.source;
    // sourceAccount
    sourceItem.sourceAccount = IResponseObject::getAttribute( node, QLatin1String( "sourceAccount" ) );
    qDebug() << "source account: " << sourceItem.sourceAccount;
    // status
    sourceItem.status = IResponseObject::getAttribute( node, QLatin1String( "status" ) );
    qDebug() << "source status: " << sourceItem.status;
    // isLocal
    if ( IResponseObject::getAttribute( node, QLatin1String( "isLocal" ) ) == QLatin1String( "true" ) )
    {
      sourceItem.isLocal = true;
      qDebug() << "is local: " << sourceItem.isLocal;
    }
    // multiroomallowed
    if ( IResponseObject::getAttribute( node, QLatin1String( "multiroomalowed" ) ) == QLatin1String( "true" ) )
    {
      sourceItem.multiroomallowed = true;
      qDebug() << "multi roo alowed: " << sourceItem.multiroomallowed;
    }
    sourceItem.Content = node->toElement().text();
    qDebug() << "entry \"Content\" has value " << sourceItem.Content;
    //
    // in die Liste aufnehmen
    //
    sourceItems.append( sourceItem );
  }

  QVector< SourceItem > HttpSourcesObject::getSourceItems() const
  {
    return sourceItems;
  }

}  // namespace bose_soundtoch_lib
