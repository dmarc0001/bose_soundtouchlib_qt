#include "iresponseobject.hpp"

namespace radio
{
  const char *IResponseObject::responseTypeNames[] = {
      "OK",   "ERROR",   "DEVICE_INFO", "NOW_PLAYING",    "VOLUME", "SOURCES", "ZONES", "BASS_CAPABILITIES",
      "BASS", "PRESETS", "GROUPS",      "PRESETS_UPDATE", "UNKNOWN"};

  IResponseObject::IResponseObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : QObject( parent ), lg( logger ), reader( xmlreader ), resultType( ResultobjectType::R_UNKNOWN )
  {
  }

  IResponseObject::~IResponseObject()
  {
    //
    // hier ist erst einmal nichts aufzuräumen
    //
  }

  ResultobjectType IResponseObject::getResultType( void )
  {
    return ( resultType );
  }

  QString IResponseObject::getResultTypeName( void ) const
  {
    return ( QString( IResponseObject::responseTypeNames[ static_cast< int >( resultType ) ] ) );
  }
}  // namespace radio
