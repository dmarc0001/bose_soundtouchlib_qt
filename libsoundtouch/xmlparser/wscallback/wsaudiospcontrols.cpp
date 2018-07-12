#include "wsaudiospcontrols.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsAudioSpControls::WsAudioSpControls
   * @param xmlreader
   * @param parent
   */
  WsAudioSpControls::WsAudioSpControls( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "audiospcontrols" ) );
    resultType = ResultobjectType::U_AUDIO_SP_CONTROLS_UNSUPPORTED;
    qDebug() << "updated (unsupported)...";
    controls = domElem->text();
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
