#include "wssourceupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsSourceUpdated::WsSourceUpdated
   * @param xmlreader
   * @param parent
   */
  WsSourceUpdated::WsSourceUpdated( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent ), updated( false )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "sourcesUpdated" ) );
    resultType = ResultobjectType::U_SOURCES_UNSUPPORTED;
    qDebug() << "updated (unsupported)...";
    updated = true;
  }

  /**
   * @brief WsSourceUpdated::~WsSourceUpdated
   */
  WsSourceUpdated::~WsSourceUpdated()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
