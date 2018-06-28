#ifndef WSBASSUPDATED_HPP
#define WSBASSUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsBassUpdated : public IResponseObject
  {
    private:
    bool updatet;  //! updated?

    public:
    explicit WsBassUpdated( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsBassUpdated() override;
    //
    // GETTER
    //
    bool getUpdatet() const;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSBASSUPDATED_HPP
