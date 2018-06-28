#include "wsbrowseupdated.hpp"

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
    source = getAttibute( reader, QLatin1String( "source" ) );
    qDebug() << "source: " << source;
    sourceAccount = getAttibute( reader, QLatin1String( "sourceAccount" ) );
    qDebug() << "source account: " << sourceAccount;
    //
    while ( reader->readNextStartElement() && !reader->hasError() )
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
