#include "wsaudiospcontrols.hpp"

namespace bose_soundtoch_lib
{
  WsAudioSpControls::WsAudioSpControls( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "audiospcontrols" ) );
    resultType = ResultobjectType::U_AUDIO_SP_CONTROLS_UNSUPPORTED;
    qDebug() << "(unsupported)...";
    controls = reader->readElementText();
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsAudioSpControls::~WsAudioSpControls()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
