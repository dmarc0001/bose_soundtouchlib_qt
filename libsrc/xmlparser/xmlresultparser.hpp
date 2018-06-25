#ifndef XMLRESULTPARSER_HPP
#define XMLRESULTPARSER_HPP

#include <qglobal.h>
#include <QObject>
#include <QXmlStreamReader>
#include <memory>
#include "../logging/Logger.hpp"
#include "httpbasscapabilitiesobject.hpp"
#include "httpbassobject.hpp"
#include "httpdeviceinfoobject.hpp"
#include "iresponseobject.hpp"
#include "httpnowplayingobject.hpp"
#include "httppresetsobject.hpp"
#include "httpresulterrorobject.hpp"
#include "httpresultokobject.hpp"
#include "httpsourcesobject.hpp"
#include "httpvolumeobject.hpp"
#include "httpzoneobject.hpp"

namespace radio
{
  class XmlResultParser : public QObject
  {
    Q_OBJECT
    private:
    std::shared_ptr< Logger > lg;
    std::unique_ptr< QXmlStreamReader > reader;
    std::shared_ptr< IResponseObject > responseObject;  //! polymorphes Objekt, enthält Ergebnise

    public:
    explicit XmlResultParser( std::shared_ptr< Logger > logger, QString &xmlString, QObject *parent = nullptr );
    ~XmlResultParser() override;
    bool hasError( void );
    QString getErrorString( void );
    std::shared_ptr< IResponseObject > getResultObject( void );

    private:
    bool parseFile( void );
  };
}  // namespace radio
#endif  // XMLRESULTPARSER_HPP
