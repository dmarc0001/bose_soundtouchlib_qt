#ifndef XMLUPDATEPARSER_HPP
#define XMLUPDATEPARSER_HPP

#include <qglobal.h>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QObject>
#include <QtDebug>
#include <memory>
#include "iresponseobject.hpp"
#include "wscallback/wsaudioproductlevelcontrols.hpp"
#include "wscallback/wsaudioproducttonecontrols.hpp"
#include "wscallback/wsaudiospcontrols.hpp"
#include "wscallback/wsbassupdated.hpp"
#include "wscallback/wsbrowseupdated.hpp"
#include "wscallback/wsconnectionstateupdated.hpp"
#include "wscallback/wserrorupdated.hpp"
#include "wscallback/wsgroupupdated.hpp"
#include "wscallback/wsinfoupdated.hpp"
#include "wscallback/wslanguageupdated.hpp"
#include "wscallback/wsnameupdated.hpp"
#include "wscallback/wsnowplayingupdate.hpp"
#include "wscallback/wsnowselectionupdated.hpp"
#include "wscallback/wspresetupdateobject.hpp"
#include "wscallback/wsrecentsupdated.hpp"
#include "wscallback/wssoundtouchsdkinfo.hpp"
#include "wscallback/wssourceupdated.hpp"
#include "wscallback/wsuseractivityupdated.hpp"
#include "wscallback/wsuserinactivityupdated.hpp"
#include "wscallback/wsvolumeupdated.hpp"
#include "wscallback/wszoneupdated.hpp"

namespace bose_soundtoch_lib
{
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT XMLUpdateParser : public QObject
  {
    Q_OBJECT
    private:
    bool wasError;                                      //! Gab es Fehler
    QDomDocument document;                              //! der XML Parser
    QDomElement docElem;                                //! das rootelement des Dokumentes
    QString errMsg;                                     //! Fehlermeldung, falls es Fehler gab
    int errLine;                                        //! Fehler in Zeile
    int errColumn;                                      //! Fehler in Spalte
    std::shared_ptr< IResponseObject > responseObject;  //! polymorphes Objekt, enthält Ergebnise

    public:
    explicit XMLUpdateParser( QString &xmlString, QObject *parent = nullptr );
    ~XMLUpdateParser() override;
    bool hasError( void );                                       //! gab es Fehler beim parsen?
    QString getErrorString( void );                              //! wenn Fehler, den Fehlerstring zurück geben
    std::shared_ptr< IResponseObject > getResultObject( void );  //! lese das Ergebnis des parsers

    private:
    void parseDom( void );  //! die xml-Daten parsen, typ feststellen
  };

}  // namespace bose_soundtoch_lib

#endif  // XMLUPDATEPARSER_HPP
