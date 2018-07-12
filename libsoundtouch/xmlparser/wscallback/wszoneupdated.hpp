#ifndef WSZONEUPDATED_HPP
#define WSZONEUPDATED_HPP

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
  class WsZoneUpdated : public IResponseObject
  {
    private:
    bool updated;

    public:
    explicit WsZoneUpdated( QDomElement *domElem, QObject *parent );
    ~WsZoneUpdated() override;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSZONEUPDATED_HPP
