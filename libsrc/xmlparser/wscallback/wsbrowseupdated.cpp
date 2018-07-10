#include "xmlparser/wscallback/wsbrowseupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsBrowseUpdated::WsBrowseUpdated
   * @param xmlreader
   * @param parent
   */
  WsBrowseUpdated::WsBrowseUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "browseUpdated" ) );
    resultType = ResultobjectType::U_BROWSE_UNSUPPORTED;
    qDebug() << "(unsupported)...";
    source = IResponseObject::getAttribute( reader, QLatin1String( "source" ) );
    qDebug() << "source: " << source;
    sourceAccount = IResponseObject::getAttribute( reader, QLatin1String( "sourceAccount" ) );
    qDebug() << "source account: " << sourceAccount;
    //
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      // elemente zuende lesen und ignorieren
    }
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
