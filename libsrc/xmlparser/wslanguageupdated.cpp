#include "wslanguageupdated.hpp"

namespace bose_soundtoch_lib
{
  WsLanguageUpdated::WsLanguageUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "languageUpdated" ) );
    resultType = ResultobjectType::U_RECENTS_UNSUPPORTED;
    lg->debug( "WsLanguageUpdated::WsLanguageUpdated (unsupported)..." );
    language = reader->readElementText();
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      // elemente zuende lesen und ignorieren
    }
  }

  WsLanguageUpdated::~WsLanguageUpdated()
  {
    lg->debug( "WsRecentsUpdate::~WsRecentsUpdate..." );
  }

}  // namespace radio
