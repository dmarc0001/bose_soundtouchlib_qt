#ifndef WSBROWSEUPDATED_HPP
#define WSBROWSEUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
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
    explicit WsBrowseUpdated( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsBrowseUpdated() override;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSBROWSEUPDATED_HPP
