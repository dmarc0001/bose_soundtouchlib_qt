#include "wszoneupdated.hpp"

namespace bose_soundtoch_lib
{
  WsZoneUpdated::WsZoneUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent ), updatet( false )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "bassUpdated" ) );
    resultType = ResultobjectType::U_ZONE;
    qDebug() << "...";
    updatet = true;
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsZoneUpdated::~WsZoneUpdated()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
