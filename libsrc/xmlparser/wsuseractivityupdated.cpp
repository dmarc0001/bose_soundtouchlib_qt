#include "wsuseractivityupdated.hpp"

namespace bose_soundtoch_lib
{
  WsUserActivityUpdated::WsUserActivityUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "userActivityUpdate" ) );
    resultType = ResultobjectType::U_USER_ACTIVITY_UPDATED_UNSUPPORTED;
    // als root tag...
    deviceId = getAttibute( reader, QLatin1String( "deviceId" ) );
    lg->debug( "WsUserActivityUpdated::WsUserActivityUpdated (unsupported)..." );
  }

  WsUserActivityUpdated::~WsUserActivityUpdated()
  {
    lg->debug( "WsUserActivityUpdated::~WsUserActivityUpdated..." );
  }

}  // namespace bose_soundtoch_lib
