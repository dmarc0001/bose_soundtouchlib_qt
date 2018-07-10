#include "xmlparser/wscallback/wszoneupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsZoneUpdated::WsZoneUpdated
   * @param xmlreader
   * @param parent
   */
  WsZoneUpdated::WsZoneUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent ), updated( false )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "bassUpdated" ) );
    resultType = ResultobjectType::U_ZONE;
    qDebug() << "(updated)...";
    updated = true;
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsZoneUpdated::~WsZoneUpdated()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
