#ifndef XMLUPDATEPARSER_HPP
#define XMLUPDATEPARSER_HPP

#include <qglobal.h>
#include <QObject>
#include <QXmlStreamReader>
#include <QtDebug>
#include <memory>
#include "iresponseobject.hpp"
#include "wsaudioproductlevelcontrols.hpp"
#include "wsaudioproducttonecontrols.hpp"
#include "wsaudiospcontrols.hpp"
#include "wsbassupdated.hpp"
#include "wsbrowseupdated.hpp"
#include "wsconnectionstateupdated.hpp"
#include "wserrorupdated.hpp"
#include "wsgroupupdated.hpp"
#include "wsinfoupdated.hpp"
#include "wslanguageupdated.hpp"
#include "wsnameupdated.hpp"
#include "wsnowplayingupdate.hpp"
#include "wsnowselectionupdated.hpp"
#include "wspresetupdateobject.hpp"
#include "wsrecentsupdated.hpp"
#include "wssoundtouchsdkinfo.hpp"
#include "wssourceupdated.hpp"
#include "wsuseractivityupdated.hpp"
#include "wsuserinactivityupdated.hpp"
#include "wsvolumeupdated.hpp"
#include "wszoneupdated.hpp"

namespace bose_soundtoch_lib
{
  class XMLUpdateParser : public QObject
  {
    Q_OBJECT
    private:
    std::unique_ptr< QXmlStreamReader > reader;         //! XML Parser, durch unique_ptr speichermanagement
    std::shared_ptr< IResponseObject > responseObject;  //! polymorphes Objekt, enthält Ergebnise

    public:
    explicit XMLUpdateParser( QString &xmlString, QObject *parent = nullptr );
    ~XMLUpdateParser() override;
    bool hasError( void );                                       //! gab es Fehler beim parsen?
    QString getErrorString( void );                              //! wenn Fehler, den Fehlerstring zurück geben
    std::shared_ptr< IResponseObject > getResultObject( void );  //! lese das Ergebnis des parsers

    private:
    bool parseFile( void );  //! die xml-Daten parsen, typ feststellen
  };

}  // namespace bose_soundtoch_lib

#endif  // XMLUPDATEPARSER_HPP
