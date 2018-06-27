#ifndef WSAUDIOPRODUCTLEVELCONTROLS_HPP
#define WSAUDIOPRODUCTLEVELCONTROLS_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsAudioProductLevelControls : public IResponseObject
  {
    private:
    QString controls;

    public:
    explicit WsAudioProductLevelControls( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsAudioProductLevelControls() override;
  };
}  // namespace radio

#endif  // WSAUDIOPRODUCTLEVELCONTROLS_HPP
