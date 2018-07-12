#ifndef SOURCESOBJECT_HPP
#define SOURCESOBJECT_HPP

#include <qglobal.h>
#include <QDomElement>
#include <QDomNode>
#include <QObject>
#include <QVector>
#include <QtDebug>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class HttpSourcesObject : public IResponseObject
  {
    private:
    QVector< SourceItem > sourceItems;

    public:
    explicit HttpSourcesObject( QDomElement *domElem, QObject *parent );
    ~HttpSourcesObject() override;
    // Getter Funktionen
    QVector< SourceItem > getSourceItems() const;

    private:
    void parseSourceItem( QDomNode *node );
  };
}  // namespace bose_soundtoch_lib
#endif  // SOURCESOBJECT_HPP
