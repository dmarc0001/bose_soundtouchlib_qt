#ifndef WSUSERINACTIVITYUPDATED_HPP
#define WSUSERINACTIVITYUPDATED_HPP

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
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT WsUserInactivityUpdated : public IResponseObject
  {
    public:
    explicit WsUserInactivityUpdated( QDomElement *domElem, QObject *parent );
    ~WsUserInactivityUpdated() override;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSUSERINACTIVITYUPDATED_HPP
