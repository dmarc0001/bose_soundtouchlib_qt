#include "wspresetupdateobject.hpp"
#include <QDateTime>

namespace bose_soundtoch_lib
{
  /**
   * @brief WsPresetUpdateObject::WsPresetUpdateObject
   * @param xmlreader
   * @param parent
   */
  WsPresetUpdateObject::WsPresetUpdateObject( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "presetUpdated" ) );
    resultType = ResultobjectType::U_PRESETS;
    qDebug() << "...";
    if ( reader->readNextStartElement() && !reader->hasError() )
    {
      //
      // das nächste element bearbeiten, welches ist es? Eigentlich nur presets
      //
      if ( reader->name() == QLatin1String( "presets" ) )
      {
        qDebug() << "tag presets found, now should follow any <preset> tags...";
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
            qWarning() << "unsupported " << reader->name().toString() << " --> " << reader->readElementText();
          }
        }
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported " << reader->name().toString() << " --> " << reader->readElementText();
      }
    }
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsPresetUpdateObject::~WsPresetUpdateObject()
  {
    qDebug() << "...";
  }

  void WsPresetUpdateObject::parseAndAddPreset( void )
  {
    DevicePreset preset;
    qDebug() << "...";
    preset.id = getAttibute( reader, QLatin1String( "id" ) );
    qDebug() << "preset id: " << preset.id;
    preset.createdOn = static_cast< qint64 >( getAttibute( reader, QLatin1String( "createdOn" ) ).toLong() );
    qDebug() << "preset created: " << preset.createdOn << " ("
             << QDateTime::fromSecsSinceEpoch( preset.createdOn ).toString( "dd.MM.yyyy hh:mm:ss" ) << ")";
    preset.updatedOn = static_cast< qint64 >( getAttibute( reader, QLatin1String( "updatedOn" ) ).toLong() );
    qDebug() << "preset updated: " << preset.updatedOn << " ("
             << QDateTime::fromSecsSinceEpoch( preset.updatedOn ).toString( "dd.MM.yyyy hh:mm:ss" ) << ")";
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
    devicePresets.append( preset );
  }

  //
  // GETTER
  //
  QVector< DevicePreset > WsPresetUpdateObject::getDevicePresets( void ) const
  {
    return ( devicePresets );
  }

}  // namespace bose_soundtoch_lib
