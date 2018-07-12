#include "httppresetsobject.hpp"
#include <QDateTime>

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpPresetsObject::HttpPresetsObject
   * @param xmlreader
   * @param parent
   */
  HttpPresetsObject::HttpPresetsObject( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "presets" ) );
    resultType = ResultobjectType::R_PRESETS;
    //
    // Device ID finden (Attribute von <info>)
    //
    qDebug() << "...";
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
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
      bose_soundtoch_lib::DevicePreset preset;
      //
      // das nächste element bearbeiten, welches ist es?
      //
      if ( currName == QLatin1String( "preset" ) )
      {
        //
        // Ein Eintrag über ein preset
        //
        qDebug() << "...";
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
        parseContentItems( &currNode, preset );
        presets.append( preset );
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag (expected preset): " << currName << " --> " << currNode.toElement().text();
      }
    }
  }

  /**
   * @brief HttpPresetsObject::~HttpPresetsObject
   */
  HttpPresetsObject::~HttpPresetsObject()
  {
    qDebug() << "...";
  }

  void HttpPresetsObject::parseContentItems( QDomNode *parentNode, DevicePreset &preset )
  {
    Q_ASSERT( parentNode->nodeName() == QLatin1String( "preset" ) );
    int contentItemCount = 0;
    //
    // erzeuge Liste mit Kindknoten (sollte hier eigetnlich nur einer sein)
    //
    QDomNodeList childNodesList( parentNode->childNodes() );
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
        preset.contentItem = std::unique_ptr< ContentItem >( new ContentItem( &currNode, this ) );
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag (expected ContentItem): " << currName << " --> " << currNode.toElement().text();
      }
    }
  }

  //
  // GETTER
  //

  QVector< bose_soundtoch_lib::DevicePreset > HttpPresetsObject::getPresets() const
  {
    return presets;
  }

}  // namespace bose_soundtoch_lib
