#ifndef XMLRESULTPARSER_HPP
#define XMLRESULTPARSER_HPP

#include <qglobal.h>
#include <QObject>
#include <QXmlStreamReader>
#include <memory>
#include "../logging/Logger.hpp"
#include "basscapabilitiesobject.hpp"
#include "bassobject.hpp"
#include "deviceinfoobject.hpp"
#include "iresponseobject.hpp"
#include "nowplayingobject.hpp"
#include "presetsobject.hpp"
#include "resulterrorobject.hpp"
#include "resultokobject.hpp"
#include "sourcesobject.hpp"
#include "volumeobject.hpp"
#include "zoneobject.hpp"

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
