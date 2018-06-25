#include "wspresetupdateobject.hpp"

namespace radio
{
  WsPresetUpdateObject::WsPresetUpdateObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "volume" ) );
    resultType = ResultobjectType::R_VOLUME;
  }

  WsPresetUpdateObject::~WsPresetUpdateObject()
  {
  }
}  // namespace radio
