#ifndef WSINFOUPDATED_HPP
#define WSINFOUPDATED_HPP

#include <qglobal.h>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT WsInfoUpdated : public IResponseObject
  {
    private:
    bool updatet;  //! update?

    public:
    explicit WsInfoUpdated( QDomElement *domElem, QObject *parent );
    ~WsInfoUpdated() override;
    //
    // GETTER
    //
    bool getUpdatet() const;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSINFOUPDATED_HPP
