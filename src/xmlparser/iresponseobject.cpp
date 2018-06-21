#include "iresponseobject.hpp"

namespace radio
{
  IResponseObject::IResponseObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : QObject( parent ), lg( logger ), reader( xmlreader ), resultType( ResultobjectType::R_UNKNOWN )
  {
  }

  ResultobjectType IResponseObject::getResultType( void )
  {
    return ( resultType );
  }

}  // namespace radio
