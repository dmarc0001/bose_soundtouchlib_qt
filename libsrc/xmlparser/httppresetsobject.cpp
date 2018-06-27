#include "httppresetsobject.hpp"

namespace bose_soundtoch_lib
{
  HttpPresetsObject::HttpPresetsObject( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "presets" ) );
    resultType = ResultobjectType::R_PRESETS;
    qDebug() << "...";
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
            preset.contentItem = std::unique_ptr< ContentItem >( new ContentItem( reader, this ) );
          }
          else
          {
            //
            // unsupportet elements
            //
            qWarning() << "unsupported tag: " << reader->name().toString() << " --> " << reader->readElementText();
          }
        }
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag: " << reader->name().toString() << " --> " << reader->readElementText();
      }
    }
  }

  HttpPresetsObject::~HttpPresetsObject()
  {
    qDebug() << "...";
  }

  QVector< bose_soundtoch_lib::DevicePreset > HttpPresetsObject::getPresets() const
  {
    return presets;
  }

}  // namespace bose_soundtoch_lib
