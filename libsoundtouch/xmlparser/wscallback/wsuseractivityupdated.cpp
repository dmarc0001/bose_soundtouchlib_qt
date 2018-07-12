#include "wsuseractivityupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsUserActivityUpdated::WsUserActivityUpdated
   * @param xmlreader
   * @param parent
   */
  WsUserActivityUpdated::WsUserActivityUpdated( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "userActivityUpdate" ) );
    qDebug() << "updated (unsupported)...";
    resultType = ResultobjectType::U_USER_ACTIVITY_UPDATED_UNSUPPORTED;
    // als root tag...
    deviceId = IResponseObject::getAttribute( domElem, QLatin1String( "deviceId" ) );
    qDebug() << "device id: " << deviceId;
  }

  /**
   * @brief WsUserActivityUpdated::~WsUserActivityUpdated
   */
  WsUserActivityUpdated::~WsUserActivityUpdated()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
