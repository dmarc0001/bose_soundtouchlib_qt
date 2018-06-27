#include "wsbrowseupdated.hpp"

namespace bose_soundtoch_lib
{
  WsBrowseUpdated::WsBrowseUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "browseUpdated" ) );
    resultType = ResultobjectType::U_BROWSE_UNSUPPORTED;
    lg->debug( "WsBrowseUpdated::WsBrowseUpdated (unsupported)..." );
    source = getAttibute( reader, QLatin1String( "source" ) );
    sourceAccount = getAttibute( reader, QLatin1String( "sourceAccount" ) );
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsBrowseUpdated::~WsBrowseUpdated()
  {
    lg->debug( "WsBrowseUpdated::~WsBrowseUpdated..." );
  }
}  // namespace radio
