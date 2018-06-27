#include "wsnowselectionupdated.hpp"

namespace bose_soundtoch_lib
{
  WsNowSelectionUpdated::WsNowSelectionUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "nowSelectionUpdated" ) );
    resultType = ResultobjectType::U_SELECTION;
    lg->debug( "WsPresetUpdateObject::WsPresetUpdateObject..." );
    //
    if ( reader->readNextStartElement() && !reader->hasError() )
    {
      //
      // das nächste element bearbeiten, welches ist es? Eigentlich nur presets
      //
      if ( reader->name() == QLatin1String( "preset" ) )
      {
        lg->debug( "WsNowSelectionUpdated::WsNowSelectionUpdated: check for attributes in \"preset\"..." );
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
            lg->debug( QString( "WsNowSelectionUpdated::WsNowSelectionUpdated: %1 -> %2" )
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
        lg->warn( QString( "WsNowSelectionUpdated::WsNowSelectionUpdated: unsupported %1 -> %2" )
                      .arg( reader->name().toString() )
                      .arg( reader->readElementText() ) );
      }
    }
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsNowSelectionUpdated::~WsNowSelectionUpdated()
  {
    lg->debug( "WsNowSelectionUpdated::~WsNowSelectionUpdated..." );
  }

  DevicePreset WsNowSelectionUpdated::getDevicePresets() const
  {
    return ( preset );
  }
}  // namespace radio
