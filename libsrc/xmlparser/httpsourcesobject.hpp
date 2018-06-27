#ifndef SOURCESOBJECT_HPP
#define SOURCESOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include "../logging/Logger.hpp"
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class HttpSourcesObject : public IResponseObject
  {
    private:
    QVector< SourceItem > sourceItems;

    public:
    explicit HttpSourcesObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpSourcesObject() override;
    // Getter Funktionen
    QVector< SourceItem > getSourceItems() const;

    private:
    void parseSourceItem( void );
  };
}  // namespace radio
#endif  // SOURCESOBJECT_HPP
