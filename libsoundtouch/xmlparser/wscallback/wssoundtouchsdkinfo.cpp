#include "wssoundtouchsdkinfo.hpp"

namespace bose_soundtoch_lib
{
  WsSoundTouchSdkInfo::WsSoundTouchSdkInfo( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "SoundTouchSdkInfo" ) );
    resultType = ResultobjectType::U_SDKINFO;
    //
    // Device ID finden
    //
    serverVersion = IResponseObject::getAttribute( domElem, QLatin1String( "serverVersion" ) );
    serverBuild = IResponseObject::getAttribute( domElem, QLatin1String( "serverBuild" ) );
    qDebug() << "sdk server version: " << serverVersion << ", server build:" << serverBuild;
  }

  WsSoundTouchSdkInfo::~WsSoundTouchSdkInfo()
  {
    qDebug() << "...";
  }

  QString WsSoundTouchSdkInfo::getServerVersion() const
  {
    return serverVersion;
  }

  QString WsSoundTouchSdkInfo::getServerBuild() const
  {
    return serverBuild;
  }

}  // namespace bose_soundtoch_lib
