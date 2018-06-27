#include "httppresetsobject.hpp"

namespace bose_soundtoch_lib
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
      bose_soundtoch_lib::DevicePreset preset;
      //
      // das nächste element bearbeiten, welches ist es?
      //
      if ( reader->name() == QLatin1String( "preset" ) )
      {
        //
        // Ein Eintrag über ein preset
        //
        lg->debug( "PresetsObject::PresetsObject: check for attributes in \"preset\"..." );
        preset.id = getAttibute( reader, QLatin1String( "id" ) );
        preset.createdOn = static_cast< qint64 >( getAttibute( reader, QLatin1String( "createdOn" ) ).toLong() );
        preset.updatedOn = static_cast< qint64 >( getAttibute( reader, QLatin1String( "updatedOn" ) ).toLong() );
        //
        // jetzt alle childknoten von preset lesen (sollte nur ContentItem sein
        //
        while ( reader->readNextStartElement() && !reader->hasError() )
        {
          if ( reader->name() == QLatin1String( "ContentItem" ) )
          {
            preset.contentItem = std::unique_ptr< ContentItem >( new ContentItem( lg, reader, this ) );
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

  QVector< bose_soundtoch_lib::DevicePreset > HttpPresetsObject::getPresets() const
  {
    return presets;
  }

}  // namespace radio
