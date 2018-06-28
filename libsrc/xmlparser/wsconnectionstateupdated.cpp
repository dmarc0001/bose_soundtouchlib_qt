#include "wsconnectionstateupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsConnectionStateUpdated::WsConnectionStateUpdated
   * @param xmlreader
   * @param parent
   */
  WsConnectionStateUpdated::WsConnectionStateUpdated( QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( xmlreader, parent ), isUp( false )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "connectionStateUpdated" ) );
    resultType = ResultobjectType::U_CONNECTION_STATE_UPDATED_UNSUPPORTED;
    qDebug() << "(unsupported)...";
    isUp = ( IResponseObject::getAttibute( reader, QLatin1String( "up" ) ) == QLatin1String( "true" ) );
    qDebug() << "connection is up: " << isUp;
    state = IResponseObject::getAttibute( reader, QLatin1String( "state" ) );
    qDebug() << "connection state: " << state;
    signal = IResponseObject::getAttibute( reader, QLatin1String( "signal" ) );
    qDebug() << "connection signal quality: " << signal;
    //
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
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
