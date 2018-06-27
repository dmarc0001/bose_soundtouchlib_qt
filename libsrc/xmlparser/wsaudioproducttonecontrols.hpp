#ifndef WSAUDIOPRODUCTTONECONTROLS_HPP
#define WSAUDIOPRODUCTTONECONTROLS_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsAudioProductToneControls : public IResponseObject
  {
    private:
    QString controls;

    public:
    explicit WsAudioProductToneControls( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsAudioProductToneControls() override;
  };
}  // namespace radio

#endif  // WSAUDIOPRODUCTTONECONTROLS_HPP
