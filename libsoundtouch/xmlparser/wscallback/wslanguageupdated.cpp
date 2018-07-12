#include "wslanguageupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsLanguageUpdated::WsLanguageUpdated
   * @param xmlreader
   * @param parent
   */
  WsLanguageUpdated::WsLanguageUpdated( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "languageUpdated" ) );
    resultType = ResultobjectType::U_RECENTS_UNSUPPORTED;
    qDebug() << "...";
    language = domElem->text();
    qDebug() << "new language: " << language;
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
