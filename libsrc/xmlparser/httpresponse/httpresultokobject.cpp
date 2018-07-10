#include "xmlparser/httpresponse/httpresultokobject.hpp"

namespace bose_soundtoch_lib
{
  HttpResultOkObject::HttpResultOkObject( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "status" ) );
    resultType = ResultobjectType::R_OK;
    status = reader->readElementText();
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
