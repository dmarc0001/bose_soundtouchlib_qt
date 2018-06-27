#include "wsinfoupdated.hpp"

namespace bose_soundtoch_lib
{
  WsInfoUpdated::WsInfoUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent ), updatet( false )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "bassUpdated" ) );
    resultType = ResultobjectType::U_INFO;
    qDebug() << "...";
    updatet = true;
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsInfoUpdated::~WsInfoUpdated()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
