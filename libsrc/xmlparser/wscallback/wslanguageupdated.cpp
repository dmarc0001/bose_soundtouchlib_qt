#include "wslanguageupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsLanguageUpdated::WsLanguageUpdated
   * @param xmlreader
   * @param parent
   */
  WsLanguageUpdated::WsLanguageUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "languageUpdated" ) );
    resultType = ResultobjectType::U_RECENTS_UNSUPPORTED;
    qDebug() << "...";
    language = reader->readElementText();
    qDebug() << "new language: " << language;
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsLanguageUpdated::~WsLanguageUpdated()
  {
    qDebug() << "...";
  }

  //
  // GETTER
  //

  QString WsLanguageUpdated::getLanguage() const
  {
    return language;
  }

}  // namespace bose_soundtoch_lib
