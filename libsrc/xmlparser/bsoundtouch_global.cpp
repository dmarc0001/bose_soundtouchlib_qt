#include "bsoundtouch_global.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief ContentItem::ContentItem erstellt aus einem ContentItem - TAG ein Objekt
   * @param xmlreader
   * @param parent
   */
  ContentItem::ContentItem( QXmlStreamReader *xmlreader, QObject *parent )
      : QObject( parent ), reader( xmlreader ), isPresetable( false )
  {
    Q_ASSERT( reader->isStartElement() &&
              ( reader->name() == QLatin1String( "ContentItem" ) || reader->name() == QLatin1String( "contentItem" ) ) );
    //
    // source/location/sourceAccount/isPresetable finden (Attribute von <ContentItem>)
    //
    qDebug() << "...";
    //
    // Attribut "source"
    //
    source = getAttibute( reader, QLatin1String( "source" ) );
    //
    // Attribut "location"
    //
    location = getAttibute( reader, QLatin1String( "location" ) );
    //
    // Attribut "sourceAccount"
    //
    sourceAccount = getAttibute( reader, QLatin1String( "sourceAccount" ) );
    //
    // Attribut "isPresetable"
    //
    if ( getAttibute( reader, QLatin1String( "isPresetable" ) ) == QLatin1String( "true" ) )
    {
      isPresetable = true;
    }
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
      }
      else if ( reader->name() == QLatin1String( "containerArt" ) )
      {
        containerArt = reader->readElementText();
      }
      else
      {
        qWarning() << "unsupported tag: " << reader->name().toString() << " --> " << reader->readElementText();
      }
    }
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
