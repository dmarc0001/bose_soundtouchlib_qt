#include "wsconnectionstateupdated.hpp"

namespace bose_soundtoch_lib
{
  WsConnectionStateUpdated::WsConnectionStateUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent ), isUp( false )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "connectionStateUpdated" ) );
    resultType = ResultobjectType::U_CONNECTION_STATE_UPDATED_UNSUPPORTED;
    lg->debug( "WsConnectionStateUpdated::WsConnectionStateUpdated (unsupported)..." );
    isUp = ( getAttibute( reader, QLatin1String( "up" ) ) == QLatin1String( "true" ) );
    state = getAttibute( reader, QLatin1String( "state" ) );
    signal = getAttibute( reader, QLatin1String( "signal" ) );

    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsConnectionStateUpdated::~WsConnectionStateUpdated()
  {
    lg->debug( "WsConnectionStateUpdated::~WsConnectionStateUpdated..." );
  }
}  // namespace radio
