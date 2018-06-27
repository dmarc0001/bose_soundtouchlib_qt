#include "bsoundtouch_global.hpp"

namespace bose_soundtoch_lib
{
  ContentItem::ContentItem( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : QObject( parent ), lg( logger ), reader( xmlreader ), isPresetable( false )
  {
    Q_ASSERT( reader->isStartElement() &&
              ( reader->name() == QLatin1String( "ContentItem" ) || reader->name() == QLatin1String( "contentItem" ) ) );
    //
    // source/location/sourceAccount/isPresetable finden (Attribute von <ContentItem>)
    //
    lg->debug( "ContentItem::ContentItem..." );
    //
    // Attribut "source"
    //
    lg->debug( "ContentItem::ContentItem: check for attribute in \"ContentItem\"..." );
    source = getAttibute( reader, QLatin1String( "source" ) );
    //
    // Attribut "location"
    //
    lg->debug( "ContentItem::ContentItem: check for attribute \"location\"..." );
    location = getAttibute( reader, QLatin1String( "location" ) );
    //
    // Attribut "sourceAccount"
    //
    lg->debug( "ContentItem::ContentItem: check for attribute \"sourceAccount\"..." );
    sourceAccount = getAttibute( reader, QLatin1String( "sourceAccount" ) );
    //
    // Attribut "isPresetable"
    //
    lg->debug( "ContentItem::ContentItem: check for attribute \"isPresetable\"..." );
    if ( getAttibute( reader, QLatin1String( "isPresetable" ) ) == QLatin1String( "true" ) )
    {
      isPresetable = true;
    }
    lg->debug( QString( "ContentItem::ContentItem: attribute \"isPresetable\" has value %1" ).arg( isPresetable ) );
    //
    // jetzt die Elemente (childknoten) lesen
    //
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      //
      // welchen Eintrag hab ich gefunden?
      //
      if ( reader->name() == QLatin1String( "itemName" ) )
      {
        itemName = reader->readElementText();
        lg->debug( QString( "ContentItem::ContentItem: itemName: %1" ).arg( itemName ) );
      }
      else if ( reader->name() == QLatin1String( "containerArt" ) )
      {
        containerArt = reader->readElementText();
        lg->debug( QString( "ContentItem::ContentItem: containerArt: %1" ).arg( containerArt ) );
      }
      else
      {
        lg->warn( QString( "ContentItem::ContentItem: unsupportet entry %1 -> %2 " )
                      .arg( reader->name().toString() )
                      .arg( reader->readElementText() ) );
      }
    }
    lg->debug( "ContentItem::ContentItem: finished." );
  }

  ContentItem::~ContentItem()
  {
  }

  QString ContentItem::getAttibute( QXmlStreamReader *reader, QLatin1String name ) const
  {
    //
    // attribute finden
    //
    QString result;
    QXmlStreamAttributes attr = reader->attributes();
    if ( attr.hasAttribute( name ) )
    {
      result = attr.value( name ).toString();
    }
    else
    {
      result.clear();
    }
    return ( result );
  }

}  // namespace bose_soundtoch_lib
