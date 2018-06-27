#ifndef RESULTOKOBJECT_HPP
#define RESULTOKOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class HttpResultOkObject : public IResponseObject
  {
    private:
    QString status;

    public:
    explicit HttpResultOkObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpResultOkObject() override;
    // Getter Funktion
    QString getStatus() const;
  };
}  // namespace radio
#endif  // RESULTOKOBJECT_HPP
