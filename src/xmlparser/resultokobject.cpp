#include "resultokobject.hpp"

namespace radio
{
  ResultOkObject::ResultOkObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "status" ) );
    resultType = ResultobjectType::R_OK;

    status = reader->readElementText();
    lg->debug( QString( "ResultOkObject::ResultOkObject: positive result status is %1" ).arg( status ) );
  }

  ResultOkObject::~ResultOkObject()
  {
    lg->debug( "ResultOkObject::~ResultOkObject..." );
  }

  QString ResultOkObject::getStatus() const
  {
    return status;
  }

}  // namespace radio
