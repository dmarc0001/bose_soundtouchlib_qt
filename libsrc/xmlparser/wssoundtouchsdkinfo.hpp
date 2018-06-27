#ifndef WSSOUNDTOUCHSDKINFO_HPP
#define WSSOUNDTOUCHSDKINFO_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsSoundTouchSdkInfo : public IResponseObject
  {
    private:
    QString serverVersion;
    QString serverBuild;

    public:
    explicit WsSoundTouchSdkInfo( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsSoundTouchSdkInfo() override;
    QString getServerVersion() const;
    QString getServerBuild() const;
  };
}  // namespace radio

#endif  // WSSOUNDTOUCHSDKINFO_HPP
