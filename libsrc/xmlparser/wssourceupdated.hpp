#ifndef WSSOURCEUPDATED_HPP
#define WSSOURCEUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsSourceUpdated : public IResponseObject
  {
    private:
    bool updated;  //! Kennzeichen, dass ein update stattfand

    public:
    explicit WsSourceUpdated( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsSourceUpdated() override;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSSOURCEUPDATED_HPP
