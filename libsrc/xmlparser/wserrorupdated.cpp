#include "wserrorupdated.hpp"

namespace bose_soundtoch_lib
{
  WsErrorUpdated::WsErrorUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "errorUpdate" ) );
    resultType = ResultobjectType::U_ERROR;
    lg->debug( "WsErrorUpdated::WsErrorUpdated..." );
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      //
      // das nächste element bearbeiten, welches ist es?
      //
      if ( reader->name() == QLatin1String( "error" ) )
      {
        error.value = getAttibute( reader, QLatin1String( "value" ) ).toInt();
        error.name = getAttibute( reader, QLatin1String( "name" ) );
        ;
        error.severity = getAttibute( reader, QLatin1String( "severity" ) );
        ;
        error.text = reader->readElementText();
      }
      else
      {
        //
        // unsupportet elements
        //
        lg->debug(
            QString( "WsErrorUpdated::WsErrorUpdated: %1 -> %2" ).arg( reader->name().toString() ).arg( reader->readElementText() ) );
      }
    }
  }

  WsErrorUpdated::~WsErrorUpdated()
  {
    lg->debug( "WsErrorUpdated::~WsErrorUpdated..." );
  }

}  // namespace radio
