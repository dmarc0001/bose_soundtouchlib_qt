#include "wsuserinactivityupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsUserInactivityUpdated::WsUserInactivityUpdated
   * @param xmlreader
   * @param parent
   */
  WsUserInactivityUpdated::WsUserInactivityUpdated( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "userInctivityUpdate" ) );
    resultType = ResultobjectType::U_USER_INACTIVITY_UPDATED_UNSUPPORTED;
    qDebug() << "updated (unsupported)...";
  }

  /**
   * @brief WsUserInactivityUpdated::~WsUserInactivityUpdated
   */
  WsUserInactivityUpdated::~WsUserInactivityUpdated()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
