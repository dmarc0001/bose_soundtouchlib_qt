#include "wslanguageupdated.hpp"

namespace bose_soundtoch_lib
{
  WsLanguageUpdated::WsLanguageUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "languageUpdated" ) );
    resultType = ResultobjectType::U_RECENTS_UNSUPPORTED;
    qDebug() << "...";
    language = reader->readElementText();
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsLanguageUpdated::~WsLanguageUpdated()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
