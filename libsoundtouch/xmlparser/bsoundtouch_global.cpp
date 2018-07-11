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
  ContentItem::ContentItem( QXmlStreamReader *xmlreader, QObject *parent )
      : QObject( parent ), reader( xmlreader ), isPresetable( false )
  {
    Q_ASSERT( reader->isStartElement() &&
              ( reader->name() == QLatin1String( "ContentItem" ) || reader->name() == QLatin1String( "contentItem" ) ) );
    //
    // source/location/sourceAccount/isPresetable finden (Attribute von
    // <ContentItem>)
    //
    qDebug() << "ContentItem...";

    //
    // Attribut "source"
    //
    source = IResponseObject::getAttribute( reader, QLatin1String( "source" ) );
    //
    // Attribute "type"
    //
    type = IResponseObject::getAttribute( reader, QLatin1String( "type" ) );
    //
    // Attribut "location"
    //
    location = IResponseObject::getAttribute( reader, QLatin1String( "location" ) );
    //
    // Attribut "sourceAccount"
    //
    sourceAccount = IResponseObject::getAttribute( reader, QLatin1String( "sourceAccount" ) );
    //
    // Attribut "isPresetable"
    //
    if ( IResponseObject::getAttribute( reader, QLatin1String( "isPresetable" ) ) == QLatin1String( "true" ) )
    {
      isPresetable = true;
    }
    //
    // jetzt die Elemente (childknoten) lesen
    //
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      qDebug().nospace() << "--------- tag " << reader->name().toString() << " -------------------";
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

  /**
   * @brief ContentItem::~ContentItem
   */
  ContentItem::~ContentItem()
  {
  }

}  // namespace bose_soundtoch_lib
