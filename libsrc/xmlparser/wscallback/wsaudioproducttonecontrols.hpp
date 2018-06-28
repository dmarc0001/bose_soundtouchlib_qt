#ifndef WSAUDIOPRODUCTTONECONTROLS_HPP
#define WSAUDIOPRODUCTTONECONTROLS_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsAudioProductToneControls : public IResponseObject
  {
    private:
    QString controls;  //! Controls?

    public:
    explicit WsAudioProductToneControls( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsAudioProductToneControls() override;
    //
    // GETTER
    //
    QString getControls() const;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSAUDIOPRODUCTTONECONTROLS_HPP
