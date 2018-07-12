#include "wsinfoupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsInfoUpdated::WsInfoUpdated
   * @param xmlreader
   * @param parent
   */
  WsInfoUpdated::WsInfoUpdated( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent ), updatet( false )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "bassUpdated" ) );
    resultType = ResultobjectType::U_INFO;
    qDebug() << "updated...";
    updatet = true;
  }

  /**
   * @brief WsInfoUpdated::~WsInfoUpdated
   */
  WsInfoUpdated::~WsInfoUpdated()
  {
    qDebug() << "...";
  }

  //
  // GETTER
  //

  bool WsInfoUpdated::getUpdatet() const
  {
    return updatet;
  }

}  // namespace bose_soundtoch_lib
