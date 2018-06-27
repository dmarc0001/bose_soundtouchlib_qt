#include "wsinfoupdated.hpp"

namespace bose_soundtoch_lib
{
  WsInfoUpdated::WsInfoUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent ), updatet( false )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "bassUpdated" ) );
    resultType = ResultobjectType::U_INFO;
    lg->debug( "WsInfoUpdated::WsInfoUpdated..." );
    updatet = true;
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsInfoUpdated::~WsInfoUpdated()
  {
    lg->debug( "WsInfoUpdated::~WsInfoUpdated..." );
  }

}  // namespace radio
