#include "wsaudioproducttonecontrols.hpp"

namespace bose_soundtoch_lib
{
  WsAudioProductToneControls::WsAudioProductToneControls( std::shared_ptr< Logger > logger,
                                                          QXmlStreamReader *xmlreader,
                                                          QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "audioproducttonecontrols" ) );
    resultType = ResultobjectType::U_RECENTS_UNSUPPORTED;
    lg->debug( "WsAudioProductToneControls::WsAudioProductToneControls (unsupported)..." );
    controls = reader->readElementText();
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsAudioProductToneControls::~WsAudioProductToneControls()
  {
    lg->debug( "WsAudioProductToneControls::~WsAudioProductToneControls..." );
  }

}  // namespace radio
