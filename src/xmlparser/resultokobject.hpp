#ifndef RESULTOKOBJECT_HPP
#define RESULTOKOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace radio
{
  class ResultOkObject : public IResponseObject
  {
    private:
    QString status;

    public:
    explicit ResultOkObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~ResultOkObject() override;
  };
}  // namespace radio
#endif  // RESULTOKOBJECT_HPP
