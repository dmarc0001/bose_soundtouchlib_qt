#include "wsnameupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsNameUpdated::WsNameUpdated
   * @param xmlreader
   * @param parent
   */
  WsNameUpdated::WsNameUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent ), updatet( false )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "nameUpdated" ) );
    resultType = ResultobjectType::U_NAME;
    qDebug() << "updated...";
    updatet = true;
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  /**
   * @brief WsNameUpdated::~WsNameUpdated
   */
  WsNameUpdated::~WsNameUpdated()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
