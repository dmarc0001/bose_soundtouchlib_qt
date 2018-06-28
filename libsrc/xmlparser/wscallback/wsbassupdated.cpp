#include "wsbassupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsBassUpdated::WsBassUpdated
   * @param xmlreader
   * @param parent
   */
  WsBassUpdated::WsBassUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent ), updatet( false )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "bassUpdated" ) );
    resultType = ResultobjectType::U_BASS;
    qDebug() << "updated...";
    updatet = true;
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
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
