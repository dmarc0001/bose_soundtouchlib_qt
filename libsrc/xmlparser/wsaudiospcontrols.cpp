#include "wsaudiospcontrols.hpp"

namespace bose_soundtoch_lib
{
  WsAudioSpControls::WsAudioSpControls( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "audiospcontrols" ) );
    resultType = ResultobjectType::U_AUDIO_SP_CONTROLS_UNSUPPORTED;
    lg->debug( "WsAudioSpControls::WsAudioSpControls (unsupported)..." );
    controls = reader->readElementText();
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsAudioSpControls::~WsAudioSpControls()
  {
    lg->debug( "WsAudioSpControls::~WsAudioSpControls..." );
  }

}  // namespace bose_soundtoch_lib
