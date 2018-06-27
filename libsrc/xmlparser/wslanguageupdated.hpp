#ifndef WSLANGUAGEUPDATED_HPP
#define WSLANGUAGEUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsLanguageUpdated : public IResponseObject
  {
    private:
    QString language;

    public:
    explicit WsLanguageUpdated( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsLanguageUpdated() override;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSLANGUAGEUPDATED_HPP
