#ifndef WSNAMEUPDATED_HPP
#define WSNAMEUPDATED_HPP

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
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT WsNameUpdated : public IResponseObject
  {
    private:
    bool updatet;  //! update marker

    public:
    explicit WsNameUpdated( QDomElement *domElem, QObject *parent );
    ~WsNameUpdated() override;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSNAMEUPDATED_HPP
