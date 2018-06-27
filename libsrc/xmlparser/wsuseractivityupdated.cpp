#include "wsuseractivityupdated.hpp"

namespace bose_soundtoch_lib
{
  WsUserActivityUpdated::WsUserActivityUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "userActivityUpdate" ) );
    qDebug() << "(unsupported)...";
    resultType = ResultobjectType::U_USER_ACTIVITY_UPDATED_UNSUPPORTED;
    // als root tag...
    deviceId = getAttibute( reader, QLatin1String( "deviceId" ) );
  }

  WsUserActivityUpdated::~WsUserActivityUpdated()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib