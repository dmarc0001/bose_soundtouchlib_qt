#ifndef WSBASSUPDATED_HPP
#define WSBASSUPDATED_HPP

#include <qglobal.h>
#include <QDomElement>
#include <QDomNode>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT WsBassUpdated : public IResponseObject
  {
    private:
    bool updatet;  //! updated?

    public:
    explicit WsBassUpdated( QDomElement *domElem, QObject *parent );
    ~WsBassUpdated() override;
    //
    // GETTER
    //
    bool getUpdatet() const;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSBASSUPDATED_HPP
