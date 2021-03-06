#ifndef WSSOUNDTOUCHSDKINFO_HPP
#define WSSOUNDTOUCHSDKINFO_HPP

#include <qglobal.h>
#include <QDomElement>
#include <QDomNode>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT WsSoundTouchSdkInfo : public IResponseObject
  {
    private:
    QString serverVersion;
    QString serverBuild;

    public:
    explicit WsSoundTouchSdkInfo( QDomElement *domElem, QObject *parent );
    ~WsSoundTouchSdkInfo() override;
    QString getServerVersion() const;
    QString getServerBuild() const;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSSOUNDTOUCHSDKINFO_HPP
