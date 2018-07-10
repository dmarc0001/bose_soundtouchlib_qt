#ifndef WSAUDIOSPCONTROLS_HPP
#define WSAUDIOSPCONTROLS_HPP

#include <qglobal.h>
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
    explicit WsAudioSpControls( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsAudioSpControls() override;
    //
    // GETTER
    //
    QString getControls() const;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSAUDIOSPCONTROLS_HPP
