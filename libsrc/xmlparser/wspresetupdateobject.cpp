#include "wspresetupdateobject.hpp"

namespace bose_soundtoch_lib
{
  WsPresetUpdateObject::WsPresetUpdateObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "presetUpdated" ) );
    resultType = ResultobjectType::U_PRESETS;
    lg->debug( "WsPresetUpdateObject::WsPresetUpdateObject..." );
    if ( reader->readNextStartElement() && !reader->hasError() )
    {
      //
      // das nächste element bearbeiten, welches ist es? Eigentlich nur presets
      //
      if ( reader->name() == QLatin1String( "presets" ) )
      {
        //
        // jetzt die einzelnen presets
        //
        while ( reader->readNextStartElement() && !reader->hasError() )
        {
          //
          // es slollten nur "preset" tags folgen
          //
          if ( reader->name() == QLatin1String( "preset" ) )
          {
            parseAndAddPreset();
          }
          else
          {
            //
            // unsupportet elements
            //
            lg->warn( QString( "WsPresetUpdateObject::WsPresetUpdateObject: unsupported %1 -> %2" )
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
        lg->warn( QString( "WsPresetUpdateObject::WsPresetUpdateObject: unsupported %1 -> %2" )
                      .arg( reader->name().toString() )
                      .arg( reader->readElementText() ) );
      }
    }
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsPresetUpdateObject::~WsPresetUpdateObject()
  {
    lg->debug( "WsPresetUpdateObject::~WsPresetUpdateObject..." );
  }

  void WsPresetUpdateObject::parseAndAddPreset( void )
  {
    DevicePreset preset;
    lg->debug( "WsPresetUpdateObject::parseAndAddPreset: check for attributes in \"preset\"..." );
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
        lg->debug( QString( "WsPresetUpdateObject::parseAndAddPreset: %1 -> %2" )
                       .arg( reader->name().toString() )
                       .arg( reader->readElementText() ) );
      }
    }
    devicePresets.append( preset );
  }

  QVector< DevicePreset > WsPresetUpdateObject::getDevicePresets( void ) const
  {
    return ( devicePresets );
  }

}  // namespace radio
