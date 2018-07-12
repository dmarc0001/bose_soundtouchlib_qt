#include "wsconnectionstateupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsConnectionStateUpdated::WsConnectionStateUpdated
   * @param xmlreader
   * @param parent
   */
  WsConnectionStateUpdated::WsConnectionStateUpdated( QDomElement *domElem, QObject *parent )
      : IResponseObject( domElem, parent ), isUp( false )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "connectionStateUpdated" ) );
    resultType = ResultobjectType::U_CONNECTION_STATE_UPDATED_UNSUPPORTED;
    qDebug() << "(unsupported)...";
    isUp = ( IResponseObject::getAttribute( domElem, QLatin1String( "up" ) ) == QLatin1String( "true" ) );
    qDebug() << "connection is up: " << isUp;
    state = IResponseObject::getAttribute( domElem, QLatin1String( "state" ) );
    qDebug() << "connection state: " << state;
    signal = IResponseObject::getAttribute( domElem, QLatin1String( "signal" ) );
    qDebug() << "connection signal quality: " << signal;
  }

  /**
   * @brief WsConnectionStateUpdated::~WsConnectionStateUpdated
   */
  WsConnectionStateUpdated::~WsConnectionStateUpdated()
  {
    qDebug() << "...";
  }

  //
  // GETTER
  //

  bool WsConnectionStateUpdated::getIsUp() const
  {
    return isUp;
  }

  QString WsConnectionStateUpdated::getState() const
  {
    return state;
  }

  QString WsConnectionStateUpdated::getSignal() const
  {
    return signal;
  }

}  // namespace bose_soundtoch_lib
