#include "wsnameupdated.hpp"

namespace bose_soundtoch_lib
{
  WsNameUpdated::WsNameUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent ), updatet( false )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "nameUpdated" ) );
    resultType = ResultobjectType::U_NAME;
    lg->debug( "WsNameUpdated::WsNameUpdated..." );
    updatet = true;
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsNameUpdated::~WsNameUpdated()
  {
    lg->debug( "WsZoneUpdated::~WsZoneUpdated..." );
  }

}  // namespace radio
