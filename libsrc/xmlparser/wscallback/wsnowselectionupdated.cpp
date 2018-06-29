#include "wsnowselectionupdated.hpp"
#include <QDateTime>

namespace bose_soundtoch_lib
{
  WsNowSelectionUpdated::WsNowSelectionUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "nowSelectionUpdated" ) );
    resultType = ResultobjectType::U_SELECTION;
    qDebug() << "...";
    //
    if ( IResponseObject::getNextStartTag( reader ) )
    {
      //
      // das nächste element bearbeiten, welches ist es? Eigentlich nur presets
      //
      if ( reader->name() == QLatin1String( "preset" ) )
      {
        qDebug() << "found tag preset...";
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
    while ( IResponseObject::getNextStartTag( reader ) )
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
