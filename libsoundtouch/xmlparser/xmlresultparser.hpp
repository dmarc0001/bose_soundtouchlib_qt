#ifndef XMLRESULTPARSER_HPP
#define XMLRESULTPARSER_HPP

#include <qglobal.h>
#include <QDomDocument>
#include <QDomElement>
#include <QObject>
#include <memory>
#include "httpresponse/httpbasscapabilitiesobject.hpp"
#include "httpresponse/httpbassobject.hpp"
#include "httpresponse/httpdeviceinfoobject.hpp"
#include "httpresponse/httpgroupobject.hpp"
#include "httpresponse/httpnowplayingobject.hpp"
#include "httpresponse/httppresetsobject.hpp"
#include "httpresponse/httpresulterrorobject.hpp"
#include "httpresponse/httpresultokobject.hpp"
#include "httpresponse/httpsourcesobject.hpp"
#include "httpresponse/httpvolumeobject.hpp"
#include "httpresponse/httpzoneobject.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class XmlResultParser : public QObject
  {
    Q_OBJECT
    private:
    std::shared_ptr< IResponseObject > responseObject;  //! polymorphes Objekt, enthält Ergebnise
    bool wasError;                                      //! Gab es Fehler
    QDomDocument document;                              //! der XML Parser
    QDomElement docElem;                                //! das rootelement des Dokumentes
    QString errMsg;                                     //! Fehlermeldung, falls es Fehler gab
    int errLine;                                        //! Fehler in Zeile
    int errColumn;                                      //! Fehler in Spalte

    public:
    explicit XmlResultParser( QString &xmlString, QObject *parent = nullptr );
    ~XmlResultParser() override;
    //
    // GETTER
    //
    bool hasError( void );
    QString getErrorString( void );
    std::shared_ptr< IResponseObject > getResultObject( void );

    private:
    void parseDom( void );  //! parse das XML und typisiere
  };
}  // namespace bose_soundtoch_lib
#endif  // XMLRESULTPARSER_HPP
