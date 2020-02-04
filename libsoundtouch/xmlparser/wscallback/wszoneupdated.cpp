#include "wszoneupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsZoneUpdated::WsZoneUpdated
   * @param xmlreader
   * @param parent
   */
  WsZoneUpdated::WsZoneUpdated( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent ), updated( false )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "zoneUpdated" ) );
    resultType = ResultobjectType::U_ZONE;
    qDebug() << "(updated)...";
    updated = true;
  }

  WsZoneUpdated::~WsZoneUpdated()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
