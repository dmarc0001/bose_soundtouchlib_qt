#ifndef SOURCESOBJECT_HPP
#define SOURCESOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include "../logging/Logger.hpp"
#include "iresponseobject.hpp"

namespace radio
{
  class SourceItem
  {
    public:
    QString source;
    QString sourceAccount;
    QString status;
    bool isLocal = false;
    bool multiroomallowed = false;
    QString Content;
  };

  class SourcesObject : public IResponseObject
  {
    private:
    QString deviceId;
    QVector< SourceItem > sourceItems;

    public:
    explicit SourcesObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    ~SourcesObject() override;

    private:
    void parseSourceItem( void );
  };
}  // namespace radio
#endif  // SOURCESOBJECT_HPP
