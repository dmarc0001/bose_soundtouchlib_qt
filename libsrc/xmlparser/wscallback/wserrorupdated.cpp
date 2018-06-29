#include "wserrorupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsErrorUpdated::WsErrorUpdated
   * @param xmlreader
   * @param parent
   */
  WsErrorUpdated::WsErrorUpdated( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "errorUpdate" ) );
    resultType = ResultobjectType::U_ERROR;
    qDebug() << "...";
    while ( IResponseObject::getNextStartTag( reader ) )
    {
      //
      // das nächste element bearbeiten, welches ist es?
      //
      if ( reader->name() == QLatin1String( "error" ) )
      {
        error.value = IResponseObject::getAttribute( reader, QLatin1String( "value" ) ).toInt();
        qDebug() << "error value: " << error.value;
        error.name = IResponseObject::getAttribute( reader, QLatin1String( "name" ) );
        qDebug() << "error name: " << error.name;
        error.severity = IResponseObject::getAttribute( reader, QLatin1String( "severity" ) );
        qDebug() << "error severity: " << error.severity;
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

  /**
   * @brief WsErrorUpdated::~WsErrorUpdated
   */
  WsErrorUpdated::~WsErrorUpdated()
  {
    qDebug() << "...";
  }

  //
  // GETTER
  //

  ResponseError WsErrorUpdated::getError() const
  {
    return error;
  }

}  // namespace bose_soundtoch_lib
