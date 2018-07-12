#include "wsaudioproducttonecontrols.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsAudioProductToneControls::WsAudioProductToneControls
   * @param xmlreader
   * @param parent
   */
  WsAudioProductToneControls::WsAudioProductToneControls( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "audioproducttonecontrols" ) );
    resultType = ResultobjectType::U_RECENTS_UNSUPPORTED;
    qDebug() << "(unsupported)...";
    controls = domElem->text();
    qDebug() << "controls: " << controls;
  }

  /**
   * @brief WsAudioProductToneControls::~WsAudioProductToneControls
   */
  WsAudioProductToneControls::~WsAudioProductToneControls()
  {
    qDebug() << "...";
  }

  //
  // GETTER
  //

  QString WsAudioProductToneControls::getControls() const
  {
    return controls;
  }

}  // namespace bose_soundtoch_lib
