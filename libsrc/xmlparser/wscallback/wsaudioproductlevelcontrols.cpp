#include "wsaudioproductlevelcontrols.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsAudioProductLevelControls::WsAudioProductLevelControls
   * @param xmlreader
   * @param parent
   */
  WsAudioProductLevelControls::WsAudioProductLevelControls( QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "audioproductlevelcontrols" ) );
    resultType = ResultobjectType::U_RECENTS_UNSUPPORTED;
    qDebug() << "(unsupported)...";
    controls = reader->readElementText();
    qDebug() << "controls: " << controls;
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      // elemente zuende lesen und ignorieren
    }
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
