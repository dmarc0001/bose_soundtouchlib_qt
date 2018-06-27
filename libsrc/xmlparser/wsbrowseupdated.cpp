#include "wsbrowseupdated.hpp"

namespace bose_soundtoch_lib
{
  WsBrowseUpdated::WsBrowseUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "browseUpdated" ) );
    resultType = ResultobjectType::U_BROWSE_UNSUPPORTED;
    qDebug() << "(unsupported)...";
    source = getAttibute( reader, QLatin1String( "source" ) );
    sourceAccount = getAttibute( reader, QLatin1String( "sourceAccount" ) );
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsBrowseUpdated::~WsBrowseUpdated()
  {
    qDebug() << "...";
  }
}  // namespace bose_soundtoch_lib
