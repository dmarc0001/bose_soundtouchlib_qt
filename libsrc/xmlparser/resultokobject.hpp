#ifndef RESULTOKOBJECT_HPP
#define RESULTOKOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include "../logging/Logger.hpp"
#include "soundtouch_response.hpp"
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
    // Getter Funktion
    QString getStatus() const;
  };
}  // namespace radio
#endif  // RESULTOKOBJECT_HPP
