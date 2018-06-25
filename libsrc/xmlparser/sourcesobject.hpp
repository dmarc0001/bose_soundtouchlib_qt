#ifndef SOURCESOBJECT_HPP
#define SOURCESOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include "../logging/Logger.hpp"
#include "soundtouch_response.hpp"
#include "iresponseobject.hpp"

namespace radio
{
  class SourcesObject : public IResponseObject
  {
    private:
    QString deviceId;
    QVector< SourceItem > sourceItems;

    public:
    explicit SourcesObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~SourcesObject() override;
    // Getter Funktionen
    QString getDeviceId() const;
    QVector< SourceItem > getSourceItems() const;

    private:
    void parseSourceItem( void );
  };
}  // namespace radio
#endif  // SOURCESOBJECT_HPP