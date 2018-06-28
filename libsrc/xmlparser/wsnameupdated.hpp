#ifndef WSNAMEUPDATED_HPP
#define WSNAMEUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsNameUpdated : public IResponseObject
  {
    private:
    bool updatet;  //! update marker

    public:
    explicit WsNameUpdated( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsNameUpdated() override;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSNAMEUPDATED_HPP
