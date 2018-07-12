#ifndef WSERRORUPDATED_HPP
#define WSERRORUPDATED_HPP

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
  class WsErrorUpdated : public IResponseObject
  {
    private:
    ResponseError error;  //! enthält die Fehlermeldungen

    public:
    explicit WsErrorUpdated( QDomElement *domElem, QObject *parent );
    ~WsErrorUpdated() override;
    //
    // GETTER
    //
    ResponseError getError() const;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSERRORUPDATED_HPP
