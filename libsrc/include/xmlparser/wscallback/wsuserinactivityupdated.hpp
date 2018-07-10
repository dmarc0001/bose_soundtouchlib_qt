#ifndef WSUSERINACTIVITYUPDATED_HPP
#define WSUSERINACTIVITYUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsUserInactivityUpdated : public IResponseObject
  {
    public:
    explicit WsUserInactivityUpdated( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsUserInactivityUpdated() override;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSUSERINACTIVITYUPDATED_HPP
