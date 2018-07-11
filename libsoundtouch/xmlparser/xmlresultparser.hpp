#ifndef XMLRESULTPARSER_HPP
#define XMLRESULTPARSER_HPP

#include <qglobal.h>
#include <QObject>
#include <QXmlStreamReader>
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
    std::unique_ptr< QXmlStreamReader > reader;         //! der XML Parser
    std::shared_ptr< IResponseObject > responseObject;  //! polymorphes Objekt, enthält Ergebnise

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
    bool parseFile( void );  //! parse das XML und typisiere
  };
}  // namespace bose_soundtoch_lib
#endif  // XMLRESULTPARSER_HPP
