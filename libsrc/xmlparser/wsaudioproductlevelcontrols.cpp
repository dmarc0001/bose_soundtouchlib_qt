#include "wsaudioproductlevelcontrols.hpp"

namespace bose_soundtoch_lib
{
  WsAudioProductLevelControls::WsAudioProductLevelControls( QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "audioproductlevelcontrols" ) );
    resultType = ResultobjectType::U_RECENTS_UNSUPPORTED;
    qDebug() << "(unsupported)...";
    controls = reader->readElementText();
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsAudioProductLevelControls::~WsAudioProductLevelControls()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
