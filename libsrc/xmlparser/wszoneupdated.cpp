#include "wszoneupdated.hpp"

namespace bose_soundtoch_lib
{
  WsZoneUpdated::WsZoneUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent ), updatet( false )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "bassUpdated" ) );
    resultType = ResultobjectType::U_ZONE;
    lg->debug( "WsZoneUpdated::WsZoneUpdated..." );
    updatet = true;
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsZoneUpdated::~WsZoneUpdated()
  {
    lg->debug( "WsZoneUpdated::~WsZoneUpdated..." );
  }

}  // namespace radio
