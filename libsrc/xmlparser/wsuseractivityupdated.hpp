#ifndef WSUSERACTIVITYUPDATED_HPP
#define WSUSERACTIVITYUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsUserActivityUpdated : public IResponseObject
  {
    public:
    explicit WsUserActivityUpdated( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsUserActivityUpdated() override;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSUSERACTIVITYUPDATED_HPP
