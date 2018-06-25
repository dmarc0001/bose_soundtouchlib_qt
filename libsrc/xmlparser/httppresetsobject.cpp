#include "httppresetsobject.hpp"

namespace radio
{
  HttpPresetsObject::HttpPresetsObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "presets" ) );
    resultType = ResultobjectType::R_PRESETS;
    lg->debug( "PresetsObject::PresetsObject..." );
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      radio::DevicePreset preset;
      //
      // das nächste element bearbeiten, welches ist es?
      //
      if ( reader->name() == QLatin1String( "preset" ) )
      {
        //
        // Ein Eintrag über ein preset
        //
        lg->debug( "PresetsObject::PresetsObject: check for attributes in \"preset\"..." );
        QXmlStreamAttributes attr = reader->attributes();
        if ( attr.hasAttribute( QLatin1String( "id" ) ) )
        {
          preset.id = attr.value( QLatin1String( "id" ) ).toString();
          lg->debug( QString( "PresetsObject::PresetsObject: preset attribute \"id\" has value %1" ).arg( preset.id ) );
        }
        else
        {
          lg->warn( "PresetsObject::PresetsObject: there is no attribute \"source\" in preset..." );
        }
        if ( attr.hasAttribute( QLatin1String( "createdOn" ) ) )
        {
          preset.createdOn = static_cast< quint64 >( attr.value( QLatin1String( "createdOn" ) ).toLong() );
          lg->debug( QString( "PresetsObject::PresetsObject: preset attribute \"createdOn\" has value %1" ).arg( preset.createdOn ) );
        }
        else
        {
          lg->warn( "PresetsObject::PresetsObject: there is no attribute \"createdOn\" in preset..." );
        }
        if ( attr.hasAttribute( QLatin1String( "updatedOn" ) ) )
        {
          preset.updatedOn = static_cast< quint64 >( attr.value( QLatin1String( "updatedOn" ) ).toLong() );
          lg->debug( QString( "PresetsObject::PresetsObject: preset attribute \"updatedOn\" has value %1" ).arg( preset.updatedOn ) );
        }
        else
        {
          lg->warn( "PresetsObject::PresetsObject: there is no attribute \"createdOn\" in preset..." );
        }
        //
        // jetzt alle childknoten von preset lesen (sollte nur ContentItem sein
        //
        while ( reader->readNextStartElement() && !reader->hasError() )
        {
          if ( reader->name() == QLatin1String( "ContentItem" ) )
          {
            parseContentItem( preset );
          }
          else
          {
            //
            // unsupportet elements
            //
            lg->debug( QString( "SourcesObject::SourcesObject: %1 -> %2" )
                           .arg( reader->name().toString() )
                           .arg( reader->readElementText() ) );
          }
        }
      }
      else
      {
        //
        // unsupportet elements
        //
        lg->debug(
            QString( "SourcesObject::SourcesObject: %1 -> %2" ).arg( reader->name().toString() ).arg( reader->readElementText() ) );
      }
    }
  }

  HttpPresetsObject::~HttpPresetsObject()
  {
    lg->debug( "PresetsObject::~PresetsObject..." );
  }

  void HttpPresetsObject::parseContentItem( radio::DevicePreset &preset )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "ContentItem" ) );
    //
    // source/location/sourceAccount/isPresetable finden (Attribute von <ContentItem>)
    //
    lg->debug( "PresetsObject::parseContentItem..." );
    //
    // Attribut "source"
    //
    lg->debug( "PresetsObject::parseContentItem: check for attribute in \"ContentItem\"..." );
    QXmlStreamAttributes attr = reader->attributes();
    if ( attr.hasAttribute( QLatin1String( "source" ) ) )
    {
      preset.contentItem.source = attr.value( QLatin1String( "source" ) ).toString();
      lg->debug( QString( "PresetsObject::parseContentItem: attribute \"source\" has value %1" ).arg( preset.contentItem.source ) );
    }
    else
    {
      lg->warn( "PresetsObject::parseContentItem: there is no attribute \"source\"..." );
    }
    //
    // Attribut "location"
    //
    lg->debug( "PresetsObject::parseContentItem: check for attribute \"location\"..." );
    if ( attr.hasAttribute( QLatin1String( "location" ) ) )
    {
      preset.contentItem.location = attr.value( QLatin1String( "location" ) ).toString();
      lg->debug(
          QString( "PresetsObject::parseContentItem: attribute \"location\" has value %1" ).arg( preset.contentItem.location ) );
    }
    else
    {
      lg->warn( "PresetsObject::parseContentItem: there is no attribute \"location\"..." );
    }
    //
    // Attribut "sourceAccount"
    //
    lg->debug( "PresetsObject::parseContentItem: check for attribute \"sourceAccount\"..." );
    if ( attr.hasAttribute( QLatin1String( "sourceAccount" ) ) )
    {
      preset.contentItem.sourceAccount = attr.value( QLatin1String( "sourceAccount" ) ).toString();
      lg->debug( QString( "PresetsObject::parseContentItem: attribute \"sourceAccount\" has value %1" )
                     .arg( preset.contentItem.sourceAccount ) );
    }
    else
    {
      lg->warn( "PresetsObject::parseContentItem: there is no attribute \"sourceAccount\"..." );
    }
    //
    // Attribut "isPresetable"
    //
    lg->debug( "PresetsObject::parseContentItem: check for attribute \"isPresetable\"..." );
    if ( attr.hasAttribute( "isPresetable" ) )
    {
      if ( attr.value( QLatin1String( "isPresetable" ) ) == "true" )
        preset.contentItem.isPresetable = true;
      else
        preset.contentItem.isPresetable = false;
      lg->debug( QString( "PresetsObject::parseContentItem: attribute \"isPresetable\" has value %1" )
                     .arg( preset.contentItem.isPresetable ) );
    }
    else
    {
      lg->warn( "PresetsObject::parseContentItem: there is no attribute \"isPresetable\"..." );
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
        preset.contentItem.itemName = reader->readElementText();
        lg->debug( QString( "PresetsObject::parseContentItem: itemName: %1" ).arg( preset.contentItem.itemName ) );
      }
      else if ( reader->name() == QLatin1String( "containerArt" ) )
      {
        preset.contentItem.containerArt = reader->readElementText();
        lg->debug( QString( "PresetsObject::parseContentItem: containerArt: %1" ).arg( preset.contentItem.containerArt ) );
      }
      else
      {
        lg->warn( QString( "PresetsObject::parseContentItem: unsupportet entry %1 -> %2 " )
                      .arg( reader->name().toString() )
                      .arg( reader->readElementText() ) );
      }
    }
    lg->debug( "PresetsObject::parseContentItem: finished." );
  }

  QVector< radio::DevicePreset > HttpPresetsObject::getPresets() const
  {
    return presets;
  }

}  // namespace radio
