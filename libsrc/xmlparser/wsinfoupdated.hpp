#ifndef WSINFOUPDATED_HPP
#define WSINFOUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsInfoUpdated : public IResponseObject
  {
    private:
    bool updatet;

    public:
    explicit WsInfoUpdated( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsInfoUpdated() override;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSINFOUPDATED_HPP
