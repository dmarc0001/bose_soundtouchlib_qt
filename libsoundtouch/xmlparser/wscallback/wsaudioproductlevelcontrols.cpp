#include "wsaudioproductlevelcontrols.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsAudioProductLevelControls::WsAudioProductLevelControls
   * @param xmlreader
   * @param parent
   */
  WsAudioProductLevelControls::WsAudioProductLevelControls( QDomElement *domElem, QObject *parent )
      : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "audioproductlevelcontrols" ) );
    resultType = ResultobjectType::U_RECENTS_UNSUPPORTED;
    qDebug() << "(unsupported)...";
    controls = domElem->text();
    qDebug() << "controls: " << controls;
  }

  /**
   * @brief WsAudioProductLevelControls::~WsAudioProductLevelControls
   */
  WsAudioProductLevelControls::~WsAudioProductLevelControls()
  {
    qDebug() << "...";
  }

  //
  // GETTER
  //
  QString WsAudioProductLevelControls::getControls() const
  {
    return controls;
  }

}  // namespace bose_soundtoch_lib
