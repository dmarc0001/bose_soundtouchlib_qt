#ifndef SOURCESOBJECT_HPP
#define SOURCESOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include "../logging/Logger.hpp"
#include "iresponseobject.hpp"
#include "soundtouch_response.hpp"

namespace radio
{
  class HttpSourcesObject : public IResponseObject
  {
    private:
    QString deviceId;
    QVector< SourceItem > sourceItems;

    public:
    explicit HttpSourcesObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~HttpSourcesObject() override;
    // Getter Funktionen
    QString getDeviceId() const;
    QVector< SourceItem > getSourceItems() const;

    private:
    void parseSourceItem( void );
  };
}  // namespace radio
#endif  // SOURCESOBJECT_HPP
