#ifndef WSUSERACTIVITYUPDATED_HPP
#define WSUSERACTIVITYUPDATED_HPP

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
  class WsUserActivityUpdated : public IResponseObject
  {
    public:
    explicit WsUserActivityUpdated( QDomElement *domElem, QObject *parent );
    ~WsUserActivityUpdated() override;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSUSERACTIVITYUPDATED_HPP
