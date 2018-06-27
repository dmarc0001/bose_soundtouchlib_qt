#ifndef BASSCAPABILITYS_HPP
#define BASSCAPABILITYS_HPP

#include <qglobal.h>
#include <QObject>
#include <QtDebug>
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class HttpBassCapabilitiesObject : public IResponseObject
  {
    private:
    bool bassAvailable = false;
    int bassMin;
    int bassMax;
    int bassDefault;

    public:
    HttpBassCapabilitiesObject( QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpBassCapabilitiesObject();
    // Funktionen um an die Daten zu kommen
    bool getBassAvailable() const;
    int getBassMin() const;
    int getBassMax() const;
    int getBassDefault() const;
  };
}  // namespace bose_soundtoch_lib
#endif  // BASSCAPABILITYS_HPP
