#include "wsnameupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsNameUpdated::WsNameUpdated
   * @param xmlreader
   * @param parent
   */
  WsNameUpdated::WsNameUpdated( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent ), updatet( false )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "nameUpdated" ) );
    resultType = ResultobjectType::U_NAME;
    qDebug() << "updated...";
    updatet = true;
  }

  /**
   * @brief WsNameUpdated::~WsNameUpdated
   */
  WsNameUpdated::~WsNameUpdated()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
