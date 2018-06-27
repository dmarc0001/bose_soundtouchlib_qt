#include "wsuserinactivityupdated.hpp"

namespace bose_soundtoch_lib
{
  WsUserInactivityUpdated::WsUserInactivityUpdated( QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "userInctivityUpdate" ) );
    resultType = ResultobjectType::U_USER_INACTIVITY_UPDATED_UNSUPPORTED;
    qDebug() << "(unsupported)...";
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsUserInactivityUpdated::~WsUserInactivityUpdated()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
