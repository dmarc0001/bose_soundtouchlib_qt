#ifndef WSAUDIOPRODUCTLEVELCONTROLS_HPP
#define WSAUDIOPRODUCTLEVELCONTROLS_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsAudioProductLevelControls : public IResponseObject
  {
    private:
    QString controls;  //! controls?

    public:
    explicit WsAudioProductLevelControls( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsAudioProductLevelControls() override;
    //
    // GETTER
    //
    QString getControls() const;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSAUDIOPRODUCTLEVELCONTROLS_HPP
