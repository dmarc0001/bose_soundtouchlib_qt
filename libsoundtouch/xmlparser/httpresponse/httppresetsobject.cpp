#include "httppresetsobject.hpp"
#include <QDateTime>

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpPresetsObject::HttpPresetsObject
   * @param xmlreader
   * @param parent
   */
  HttpPresetsObject::HttpPresetsObject( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "presets" ) );
    resultType = ResultobjectType::R_PRESETS;
    qDebug() << "...";
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( IResponseObject::getNextStartTag( reader ) )
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
        qDebug() << "...";
        preset.id = IResponseObject::getAttribute( reader, QLatin1String( "id" ) );
        qDebug() << "preset id: " << preset.id;
        preset.createdOn = static_cast< qint64 >( IResponseObject::getAttribute( reader, QLatin1String( "createdOn" ) ).toLong() );
        qDebug() << "preset created: " << preset.createdOn << " ("
                 << QDateTime::fromSecsSinceEpoch( preset.createdOn ).toString( "dd.MM.yyyy hh:mm:ss" ) << ")";
        preset.updatedOn = static_cast< qint64 >( IResponseObject::getAttribute( reader, QLatin1String( "updatedOn" ) ).toLong() );
        qDebug() << "preset updated: " << preset.updatedOn << " ("
                 << QDateTime::fromSecsSinceEpoch( preset.updatedOn ).toString( "dd.MM.yyyy hh:mm:ss" ) << ")";
        //
        // jetzt alle childknoten von preset lesen (sollte nur ContentItem sein
        //
        while ( IResponseObject::getNextStartTag( reader ) )
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
            qWarning() << "unsupported tag (expected ContentItem): " << reader->name().toString() << " --> "
                       << reader->readElementText();
          }
        }
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag (expected preset): " << reader->name().toString() << " --> " << reader->readElementText();
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

  //
  // GETTER
  //

  QVector< bose_soundtoch_lib::DevicePreset > HttpPresetsObject::getPresets() const
  {
    return presets;
  }

}  // namespace bose_soundtoch_lib
