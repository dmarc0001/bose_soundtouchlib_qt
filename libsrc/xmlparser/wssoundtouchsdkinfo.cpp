#include "wssoundtouchsdkinfo.hpp"

namespace bose_soundtoch_lib
{
  WsSoundTouchSdkInfo::WsSoundTouchSdkInfo( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "SoundTouchSdkInfo" ) );
    resultType = ResultobjectType::U_SDKINFO;
    //
    // Device ID finden (Attribute von <updates>)
    //
    serverVersion = getAttibute( reader, QLatin1String( "serverVersion" ) );
    serverBuild = getAttibute( reader, QLatin1String( "serverBuild" ) );
    lg->debug( QString( "WsSoundTouchSdkInfo::WsSoundTouchSdkInfo: server version: <%1>, server build: <%2>" )
                   .arg( serverVersion )
                   .arg( serverBuild ) );
  }

  WsSoundTouchSdkInfo::~WsSoundTouchSdkInfo()
  {
    lg->debug( "WsSoundTouchSdkInfo::~WsSoundTouchSdkInfo" );
  }

  QString WsSoundTouchSdkInfo::getServerVersion() const
  {
    return serverVersion;
  }

  QString WsSoundTouchSdkInfo::getServerBuild() const
  {
    return serverBuild;
  }

}  // namespace radio
