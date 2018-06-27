#include "wssourceupdated.hpp"

namespace bose_soundtoch_lib
{
  WsSourceUpdated::WsSourceUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent ), updated( false )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "sourcesUpdated" ) );
    resultType = ResultobjectType::U_SOURCES_UNSUPPORTED;
    if ( reader->name() == QLatin1String( "sourcesUpdate" ) )
    {
      updated = true;
    }
    lg->debug( "WsSourceUpdated::WsSourceUpdated (unsupported)..." );
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsSourceUpdated::~WsSourceUpdated()
  {
    lg->debug( "WsSourceUpdated::~WsSourceUpdated..." );
  }

}  // namespace radio
