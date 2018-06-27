#ifndef WSCONNECTIONSTATEUPDATED_HPP
#define WSCONNECTIONSTATEUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsConnectionStateUpdated : public IResponseObject
  {
    private:
    bool isUp;
    QString state;
    QString signal;

    public:
    explicit WsConnectionStateUpdated( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsConnectionStateUpdated() override;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSCONNECTIONSTATEUPDATED_HPP
