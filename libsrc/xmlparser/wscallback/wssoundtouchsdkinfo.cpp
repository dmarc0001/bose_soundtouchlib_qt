#include "wscallback/wssoundtouchsdkinfo.hpp"

namespace bose_soundtoch_lib
{
  WsSoundTouchSdkInfo::WsSoundTouchSdkInfo( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "SoundTouchSdkInfo" ) );
    resultType = ResultobjectType::U_SDKINFO;
    //
    // Device ID finden (Attribute von <updates>)
    //
    serverVersion = IResponseObject::getAttribute( reader, QLatin1String( "serverVersion" ) );
    serverBuild = IResponseObject::getAttribute( reader, QLatin1String( "serverBuild" ) );
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
