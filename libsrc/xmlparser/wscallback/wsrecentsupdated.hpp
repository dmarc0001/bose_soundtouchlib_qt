#ifndef WSRECENTSUPDATE_HPP
#define WSRECENTSUPDATE_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QVector>
#include <QtDebug>
#include <memory>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsRecentsUpdated : public IResponseObject
  {
    private:
    QVector< DeviceRecent > recents;  //! Liste der zurückliegend gespielten Sender

    public:
    explicit WsRecentsUpdated( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsRecentsUpdated() override;

    private:
    void readRecent( void );  //! einen Eintag lesen und der Liste zufügen
  };
}  // namespace bose_soundtoch_lib

#endif  // WSRECENTSUPDATE_HPP
