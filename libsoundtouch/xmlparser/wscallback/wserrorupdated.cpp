#include "wserrorupdated.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief WsErrorUpdated::WsErrorUpdated
   * @param xmlreader
   * @param parent
   */
  WsErrorUpdated::WsErrorUpdated( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "errorUpdate" ) );
    resultType = ResultobjectType::U_ERROR;
    qDebug() << "...";
    QDomNode rnode( domElem->firstChild() );
    if ( !rnode.isNull() && rnode.nodeName() == QLatin1String( "error" ) )
    {
      error.value = IResponseObject::getAttribute( &rnode, QLatin1String( "value" ) ).toInt();
      qDebug() << "error value: " << error.value;
      error.name = IResponseObject::getAttribute( &rnode, QLatin1String( "name" ) );
      qDebug() << "error name: " << error.name;
      error.severity = IResponseObject::getAttribute( &rnode, QLatin1String( "severity" ) );
      qDebug() << "error severity: " << error.severity;
      error.text = rnode.toElement().text();
    }
    else
    {
      if ( rnode.isNull() )
      {
        qWarning() << "no child node found!";
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag: " << rnode.nodeName() << " --> " << rnode.toElement().text();
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
