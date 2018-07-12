#include "httpresultokobject.hpp"

namespace bose_soundtoch_lib
{
  HttpResultOkObject::HttpResultOkObject( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "status" ) );
    resultType = ResultobjectType::R_OK;
    status = domElem->text();
    qDebug() << "positive result status is " << status;
  }

  HttpResultOkObject::~HttpResultOkObject()
  {
    qDebug() << "...";
  }

  QString HttpResultOkObject::getStatus() const
  {
    return status;
  }

}  // namespace bose_soundtoch_lib
