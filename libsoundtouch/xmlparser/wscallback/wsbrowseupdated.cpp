#include "wsbrowseupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsBrowseUpdated::WsBrowseUpdated
   * @param xmlreader
   * @param parent
   */
  WsBrowseUpdated::WsBrowseUpdated( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "browseUpdated" ) );
    resultType = ResultobjectType::U_BROWSE_UNSUPPORTED;
    qDebug() << "(unsupported)...";
    source = IResponseObject::getAttribute( domElem, QLatin1String( "source" ) );
    qDebug() << "source: " << source;
    sourceAccount = IResponseObject::getAttribute( domElem, QLatin1String( "sourceAccount" ) );
    qDebug() << "source account: " << sourceAccount;
    //
  }

  /**
   * @brief WsBrowseUpdated::~WsBrowseUpdated
   */
  WsBrowseUpdated::~WsBrowseUpdated()
  {
    qDebug() << "...";
  }

  //
  // GETTER
  //

  QString WsBrowseUpdated::getSource() const
  {
    return source;
  }

  QString WsBrowseUpdated::getSourceAccount() const
  {
    return sourceAccount;
  }

}  // namespace bose_soundtoch_lib
