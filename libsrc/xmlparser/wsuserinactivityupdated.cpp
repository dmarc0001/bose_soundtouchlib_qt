#include "wsuserinactivityupdated.hpp"

namespace bose_soundtoch_lib
{
  WsUserInactivityUpdated::WsUserInactivityUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "userInctivityUpdate" ) );
    resultType = ResultobjectType::U_USER_INACTIVITY_UPDATED_UNSUPPORTED;
    lg->debug( "WsUserInactivityUpdated::WsUserInactivityUpdated (unsupported)..." );
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsUserInactivityUpdated::~WsUserInactivityUpdated()
  {
    lg->debug( "WsRecentsUpdate::~WsRecentsUpdate..." );
  }

}  // namespace radio
