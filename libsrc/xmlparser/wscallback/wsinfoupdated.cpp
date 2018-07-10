#include "xmlparser/wscallback/wsinfoupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsInfoUpdated::WsInfoUpdated
   * @param xmlreader
   * @param parent
   */
  WsInfoUpdated::WsInfoUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent ), updatet( false )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "bassUpdated" ) );
    resultType = ResultobjectType::U_INFO;
    qDebug() << "updated...";
    updatet = true;
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      // elemente zuende lesen und ignorieren
    }
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
