#ifndef BASSCAPABILITYS_HPP
#define BASSCAPABILITYS_HPP

#include <qglobal.h>
#include <QObject>
#include <QtDebug>
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class HttpBassCapabilitiesObject : public IResponseObject
  {
    private:
    bool bassAvailable = false;  //! Bassfunktion verfügbar?
    int bassMin;                 //! Minimale Bass einstellung
    int bassMax;                 //! Maximale Bass einstellung
    int bassDefault;             //! Default wert

    public:
    HttpBassCapabilitiesObject( QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpBassCapabilitiesObject();
    // Funktionen um an die Daten zu kommen
    // GETTERS:
    bool getBassAvailable() const;  //! ist Bass Verfügbar?
    int getBassMin() const;         //! Bass Minimum?
    int getBassMax() const;         //! Bass Maximum?
    int getBassDefault() const;     //! Defaultwert Bass
  };
}  // namespace bose_soundtoch_lib
#endif  // BASSCAPABILITYS_HPP
