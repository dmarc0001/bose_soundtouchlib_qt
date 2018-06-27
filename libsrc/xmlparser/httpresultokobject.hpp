#ifndef RESULTOKOBJECT_HPP
#define RESULTOKOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QtDebug>
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class HttpResultOkObject : public IResponseObject
  {
    private:
    QString status;

    public:
    explicit HttpResultOkObject( QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpResultOkObject() override;
    // Getter Funktion
    QString getStatus() const;
  };
}  // namespace bose_soundtoch_lib
#endif  // RESULTOKOBJECT_HPP
