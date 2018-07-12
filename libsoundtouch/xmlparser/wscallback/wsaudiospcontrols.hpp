#ifndef WSAUDIOSPCONTROLS_HPP
#define WSAUDIOSPCONTROLS_HPP

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
  class WsAudioSpControls : public IResponseObject
  {
    private:
    QString controls;  //! Controls...

    public:
    explicit WsAudioSpControls( QDomElement *domElem, QObject *parent );
    ~WsAudioSpControls() override;
    //
    // GETTER
    //
    QString getControls() const;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSAUDIOSPCONTROLS_HPP
