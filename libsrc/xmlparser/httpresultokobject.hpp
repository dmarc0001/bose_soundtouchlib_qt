#ifndef RESULTOKOBJECT_HPP
#define RESULTOKOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include "../logging/Logger.hpp"
#include "iresponseobject.hpp"
#include "soundtouch_response.hpp"

namespace radio
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
