#include "wsbassupdated.hpp"

namespace bose_soundtoch_lib
{
  WsBassUpdated::WsBassUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent ), updatet( false )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "bassUpdated" ) );
    resultType = ResultobjectType::U_BASS;
    lg->debug( "WsBassUpdated::WsBassUpdated..." );
    updatet = true;
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsBassUpdated::~WsBassUpdated()
  {
    lg->debug( "WsBassUpdated::~WsBassUpdated..." );
  }

}  // namespace radio
