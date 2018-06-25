#include "httpresultokobject.hpp"

namespace radio
{
  HttpResultOkObject::HttpResultOkObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "status" ) );
    resultType = ResultobjectType::R_OK;

    status = reader->readElementText();
    lg->debug( QString( "ResultOkObject::ResultOkObject: positive result status is %1" ).arg( status ) );
  }

  HttpResultOkObject::~HttpResultOkObject()
  {
    lg->debug( "ResultOkObject::~ResultOkObject..." );
  }

  QString HttpResultOkObject::getStatus() const
  {
    return status;
  }

}  // namespace radio
