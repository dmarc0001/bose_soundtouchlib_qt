#include "wsaudioproducttonecontrols.hpp"

namespace bose_soundtoch_lib
{
  WsAudioProductToneControls::WsAudioProductToneControls( QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "audioproducttonecontrols" ) );
    resultType = ResultobjectType::U_RECENTS_UNSUPPORTED;
    qDebug() << "(unsupported)...";
    controls = reader->readElementText();
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsAudioProductToneControls::~WsAudioProductToneControls()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
