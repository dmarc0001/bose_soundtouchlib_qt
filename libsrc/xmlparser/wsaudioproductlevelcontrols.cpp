#include "wsaudioproductlevelcontrols.hpp"

namespace bose_soundtoch_lib
{
  WsAudioProductLevelControls::WsAudioProductLevelControls( std::shared_ptr< Logger > logger,
                                                            QXmlStreamReader *xmlreader,
                                                            QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "audioproductlevelcontrols" ) );
    resultType = ResultobjectType::U_RECENTS_UNSUPPORTED;
    lg->debug( "WsAudioProductLevelControls::WsAudioProductLevelControls (unsupported)..." );
    controls = reader->readElementText();
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsAudioProductLevelControls::~WsAudioProductLevelControls()
  {
    lg->debug( "WsAudioProductLevelControls::~WsAudioProductLevelControls..." );
  }

}  // namespace radio
