#include "wspresetupdateobject.hpp"
#include <QDateTime>

namespace bose_soundtoch_lib
{
  /**
   * @brief WsPresetUpdateObject::WsPresetUpdateObject
   * @param xmlreader
   * @param parent
   */
  WsPresetUpdateObject::WsPresetUpdateObject( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "presetUpdated" ) );
    resultType = ResultobjectType::U_PRESETS;
    qDebug() << "...";
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
      if ( currName == QLatin1String( "presets" ) )
      {
        qDebug() << "tag presets found, now should follow any <preset> tags...";
        //
        // jetzt die einzelnen presets
        //
        parsePresets( &currNode );
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported " << currName << " --> " << currNode.toElement().text();
      }
    }
  }

  WsPresetUpdateObject::~WsPresetUpdateObject()
  {
    qDebug() << "...";
  }

  void WsPresetUpdateObject::parsePresets( QDomNode *parentNode )
  {
    Q_ASSERT( parentNode->nodeName() == QLatin1String( "presets" ) );
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
      if ( currName == QLatin1String( "preset" ) )
      {
        parseAndAddPreset( &currNode );
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported " << currName << " --> " << currNode.toElement().text();
      }
    }
  }

  void WsPresetUpdateObject::parseAndAddPreset( QDomNode *parentNode )
  {
    Q_ASSERT( parentNode->nodeName() == QLatin1String( "preset" ) );
    DevicePreset preset;
    int countContentItem = 0;
    qDebug() << "...";
    preset.id = IResponseObject::getAttribute( parentNode, QLatin1String( "id" ) );
    qDebug() << "preset id: " << preset.id;
    preset.createdOn = static_cast< qint64 >( IResponseObject::getAttribute( parentNode, QLatin1String( "createdOn" ) ).toLong() );
    qDebug() << "preset created: " << preset.createdOn << " ("
             << QDateTime::fromSecsSinceEpoch( preset.createdOn ).toString( "dd.MM.yyyy hh:mm:ss" ) << ")";
    preset.updatedOn = static_cast< qint64 >( IResponseObject::getAttribute( parentNode, QLatin1String( "updatedOn" ) ).toLong() );
    qDebug() << "preset updated: " << preset.updatedOn << " ("
             << QDateTime::fromSecsSinceEpoch( preset.updatedOn ).toString( "dd.MM.yyyy hh:mm:ss" ) << ")";
    //
    // jetzt alle childknoten von preset lesen (sollte nur ContentItem sein
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
    devicePresets.append( preset );
  }

  //
  // GETTER
  //
  QVector< DevicePreset > WsPresetUpdateObject::getDevicePresets( void ) const
  {
    return ( devicePresets );
  }

}  // namespace bose_soundtoch_lib
