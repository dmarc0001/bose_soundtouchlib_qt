#ifndef WSBROWSEUPDATED_HPP
#define WSBROWSEUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <memory>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsBrowseUpdated : public IResponseObject
  {
    private:
    QString source;
    QString sourceAccount;

    public:
    explicit WsBrowseUpdated( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~WsBrowseUpdated() override;
  };
}  // namespace radio

#endif  // WSBROWSEUPDATED_HPP
