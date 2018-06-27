#ifndef WSNOWPLAYINGUPDATE_HPP
#define WSNOWPLAYINGUPDATE_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QVector>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "httpnowplayingobject.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  using WsNowPlayingUpdate = HttpNowPlayingObject;
}  // namespace radio

#endif  // WSNOWPLAYINGUPDATE_HPP
