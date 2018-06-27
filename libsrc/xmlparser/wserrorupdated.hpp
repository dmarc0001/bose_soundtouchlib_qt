#ifndef WSERRORUPDATED_HPP
#define WSERRORUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsErrorUpdated : public IResponseObject
  {
    private:
    ResponseError error;

    public:
    explicit WsErrorUpdated( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsErrorUpdated() override;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSERRORUPDATED_HPP
