#include "wsnameupdated.hpp"

namespace bose_soundtoch_lib
{
  WsNameUpdated::WsNameUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent ), updatet( false )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "nameUpdated" ) );
    resultType = ResultobjectType::U_NAME;
    qDebug() << "...";
    updatet = true;
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsNameUpdated::~WsNameUpdated()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
