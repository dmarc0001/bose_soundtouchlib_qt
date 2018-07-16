#ifndef WSCONNECTIONSTATEUPDATED_HPP
#define WSCONNECTIONSTATEUPDATED_HPP

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
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT WsConnectionStateUpdated : public IResponseObject
  {
    private:
    bool isUp;       //! is connection up?
    QString state;   //! verbuindungsstatus
    QString signal;  //! signalqualität

    public:
    explicit WsConnectionStateUpdated( QDomElement *domElem, QObject *parent );
    ~WsConnectionStateUpdated() override;
    //
    // GETTER
    //
    bool getIsUp() const;
    QString getState() const;
    QString getSignal() const;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSCONNECTIONSTATEUPDATED_HPP
