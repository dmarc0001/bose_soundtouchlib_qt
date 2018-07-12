#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief ContentItem::ContentItem erstellt aus einem ContentItem - TAG ein
   * Objekt
   * @param xmlreader
   * @param parent
   */
  ContentItem::ContentItem( QDomNode *node, QObject *parent ) : QObject( parent ), node( node ), isPresetable( false )
  {
    Q_ASSERT( node->nodeName() == QLatin1String( "ContentItem" ) || node->nodeName() == QLatin1String( "contentItem" ) );
    //
    // source/location/sourceAccount/isPresetable finden (Attribute von
    // <ContentItem>)
    //
    qDebug() << "ContentItem...";
    //
    // Attribut "source"
    //
    source = IResponseObject::getAttribute( node, QLatin1String( "source" ) );
    qDebug() << "ContentItem:source: " << source;
    //
    // Attribute "type"
    //
    type = IResponseObject::getAttribute( node, QLatin1String( "type" ) );
    qDebug() << "ContentItem:type: " << type;
    //
    // Attribut "location"
    //
    location = IResponseObject::getAttribute( node, QLatin1String( "location" ) );
    qDebug() << "ContentItem:location: " << location;
    //
    // Attribut "sourceAccount"
    //
    sourceAccount = IResponseObject::getAttribute( node, QLatin1String( "sourceAccount" ) );
    qDebug() << "ContentItem:sourceAccount: " << sourceAccount;
    //
    // Attribut "isPresetable"
    //
    if ( IResponseObject::getAttribute( node, QLatin1String( "isPresetable" ) ) == QLatin1String( "true" ) )
    {
      isPresetable = true;
      qDebug() << "ContentItem:isPresetable: true";
    }
    //
    // jetzt die Elemente (childknoten) lesen
    //
    QDomNodeList childNodesList( node->childNodes() );
    for ( int nodeIdx = 0; nodeIdx < childNodesList.length(); nodeIdx++ )
    {
      QDomNode currNode( childNodesList.item( nodeIdx ) );
      if ( currNode.isNull() )
        continue;
      QString currName( currNode.nodeName() );
      //
      // welchen Eintrag hab ich gefunden?
      //
      if ( currName == QLatin1String( "itemName" ) )
      {
        itemName = currNode.toElement().text();
        qDebug() << "ContentItem:itemName: " << itemName;
      }
      else if ( currName == QLatin1String( "containerArt" ) )
      {
        containerArt = currNode.toElement().text();
        qDebug() << "ContentItem:containerArt: " << containerArt;
      }
      else
      {
        qWarning() << "unsupported tag: " << currName << " --> " << currNode.toElement().text();
      }
    }
  }

  /**
   * @brief ContentItem::~ContentItem
   */
  ContentItem::~ContentItem()
  {
  }

}  // namespace bose_soundtoch_lib
