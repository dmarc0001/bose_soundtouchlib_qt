#include "wsbassupdated.hpp"

namespace bose_soundtoch_lib
{
  WsBassUpdated::WsBassUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent ), updatet( false )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "bassUpdated" ) );
    resultType = ResultobjectType::U_BASS;
    qDebug() << "...";
    updatet = true;
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsBassUpdated::~WsBassUpdated()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
