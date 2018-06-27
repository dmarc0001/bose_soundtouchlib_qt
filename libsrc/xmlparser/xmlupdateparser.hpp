#ifndef XMLUPDATEPARSER_HPP
#define XMLUPDATEPARSER_HPP

#include <qglobal.h>
#include <QObject>
#include <QXmlStreamReader>
#include <memory>
#include "../logging/Logger.hpp"
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
    std::shared_ptr< Logger > lg;
    std::unique_ptr< QXmlStreamReader > reader;
    std::shared_ptr< IResponseObject > responseObject;  //! polymorphes Objekt, enthält Ergebnise

    public:
    explicit XMLUpdateParser( std::shared_ptr< Logger > logger, QString &xmlString, QObject *parent = nullptr );
    ~XMLUpdateParser() override;
    bool hasError( void );
    QString getErrorString( void );
    std::shared_ptr< IResponseObject > getResultObject( void );

    private:
    bool parseFile( void );
    QString getAttibute( QXmlStreamReader *reader, QLatin1String name ) const;
  };

}  // namespace bose_soundtoch_lib

#endif  // XMLUPDATEPARSER_HPP
