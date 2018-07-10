#include "wscallback/wsaudioproducttonecontrols.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsAudioProductToneControls::WsAudioProductToneControls
   * @param xmlreader
   * @param parent
   */
  WsAudioProductToneControls::WsAudioProductToneControls( QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "audioproducttonecontrols" ) );
    resultType = ResultobjectType::U_RECENTS_UNSUPPORTED;
    qDebug() << "(unsupported)...";
    controls = reader->readElementText();
    qDebug() << "controls: " << controls;
    //
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      // elemente zuende lesen und ignorieren
    }
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
