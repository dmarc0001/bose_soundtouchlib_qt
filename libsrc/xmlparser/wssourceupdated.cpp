#include "wssourceupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsSourceUpdated::WsSourceUpdated
   * @param xmlreader
   * @param parent
   */
  WsSourceUpdated::WsSourceUpdated( QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( xmlreader, parent ), updated( false )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "sourcesUpdated" ) );
    resultType = ResultobjectType::U_SOURCES_UNSUPPORTED;
    qDebug() << "updated (unsupported)...";
    if ( reader->name() == QLatin1String( "sourcesUpdate" ) )
    {
      updated = true;
    }
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  /**
   * @brief WsSourceUpdated::~WsSourceUpdated
   */
  WsSourceUpdated::~WsSourceUpdated()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
