#include "wsbassupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsBassUpdated::WsBassUpdated
   * @param xmlreader
   * @param parent
   */
  WsBassUpdated::WsBassUpdated( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent ), updatet( false )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "bassUpdated" ) );
    resultType = ResultobjectType::U_BASS;
    qDebug() << "updated...";
    updatet = true;
  }

  /**
   * @brief WsBassUpdated::~WsBassUpdated
   */
  WsBassUpdated::~WsBassUpdated()
  {
    qDebug() << "...";
  }

  //
  // GETTER
  //

  bool WsBassUpdated::getUpdatet() const
  {
    return updatet;
  }

}  // namespace bose_soundtoch_lib
