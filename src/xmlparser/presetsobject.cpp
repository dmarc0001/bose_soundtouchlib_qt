#include "presetsobject.hpp"

namespace radio
{
  PresetsObject::PresetsObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
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
      //
      // das nächste element bearbeiten, welches ist es?
      //
      if ( reader->name() == QLatin1String( "sourceItem" ) )
      {
        //
        // Ein Eintrag über eine source
        //
        parseSourceItem();
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

  PresetsObject::~PresetsObject()
  {
    lg->debug( "PresetsObject::~PresetsObject..." );
  }

}  // namespace radio
