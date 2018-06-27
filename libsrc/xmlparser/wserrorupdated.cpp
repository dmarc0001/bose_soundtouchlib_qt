#include "wserrorupdated.hpp"

namespace bose_soundtoch_lib
{
  WsErrorUpdated::WsErrorUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "errorUpdate" ) );
    resultType = ResultobjectType::U_ERROR;
    qDebug() << "...";
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
        qWarning() << "unsupported tag: " << reader->name().toString() << " --> " << reader->readElementText();
      }
    }
  }

  WsErrorUpdated::~WsErrorUpdated()
  {
    qDebug() << "...";
  }

}  // namespace bose_soundtoch_lib
