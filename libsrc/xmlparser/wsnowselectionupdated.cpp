#include "wsnowselectionupdated.hpp"

namespace bose_soundtoch_lib
{
  WsNowSelectionUpdated::WsNowSelectionUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "nowSelectionUpdated" ) );
    resultType = ResultobjectType::U_SELECTION;
    qDebug() << "...";
    //
    if ( reader->readNextStartElement() && !reader->hasError() )
    {
      //
      // das nächste element bearbeiten, welches ist es? Eigentlich nur presets
      //
      if ( reader->name() == QLatin1String( "preset" ) )
      {
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
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsNowSelectionUpdated::~WsNowSelectionUpdated()
  {
    qDebug() << "...";
  }

  DevicePreset WsNowSelectionUpdated::getDevicePresets() const
  {
    return ( preset );
  }
}  // namespace bose_soundtoch_lib
