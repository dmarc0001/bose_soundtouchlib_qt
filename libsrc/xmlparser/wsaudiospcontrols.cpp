#include "wsaudiospcontrols.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsAudioSpControls::WsAudioSpControls
   * @param xmlreader
   * @param parent
   */
  WsAudioSpControls::WsAudioSpControls( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "audiospcontrols" ) );
    resultType = ResultobjectType::U_AUDIO_SP_CONTROLS_UNSUPPORTED;
    qDebug() << "updated (unsupported)...";
    controls = reader->readElementText();
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  /**
   * @brief WsAudioSpControls::~WsAudioSpControls
   */
  WsAudioSpControls::~WsAudioSpControls()
  {
    qDebug() << "...";
  }

  //
  // GETTER
  //

  QString WsAudioSpControls::getControls() const
  {
    return controls;
  }

}  // namespace bose_soundtoch_lib
