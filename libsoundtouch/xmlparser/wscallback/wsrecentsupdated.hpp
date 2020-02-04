#ifndef WSRECENTSUPDATE_HPP
#define WSRECENTSUPDATE_HPP

#include <qglobal.h>
#include <QDomElement>
#include <QDomNode>
#include <QObject>
#include <QString>
#include <QVector>
#include <QtDebug>
#include <memory>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT WsRecentsUpdated : public IResponseObject
  {
    private:
    QVector< DeviceRecent > recents;  //! Liste der zurückliegend gespielten Sender

    public:
    explicit WsRecentsUpdated( QDomElement *domElem, QObject *parent );
    ~WsRecentsUpdated() override;

    private:
    void readRecent( QDomNode *node );  //! einen Eintag lesen und der Liste zufügen
  };
}  // namespace bose_soundtoch_lib

#endif  // WSRECENTSUPDATE_HPP
