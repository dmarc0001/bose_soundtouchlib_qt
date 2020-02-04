#ifndef WSPRESETUPDATEOBJECT_HPP
#define WSPRESETUPDATEOBJECT_HPP

#include <qglobal.h>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QObject>
#include <QString>
#include <QVector>
#include <QtDebug>
#include <memory>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT WsPresetUpdateObject : public IResponseObject
  {
    private:
    QVector< DevicePreset > devicePresets;  //! Liste mit Presets des Gerätes/der Geräte. Aktuell 6 stück möglich

    public:
    explicit WsPresetUpdateObject( QDomElement *domElem, QObject *parent );
    ~WsPresetUpdateObject() override;
    //
    // GETTER
    //
    QVector< DevicePreset > getDevicePresets( void ) const;

    private:
    void parsePresets( QDomNode *parentNode );       //! jedes preset finden
    void parseAndAddPreset( QDomNode *parentNode );  //! einien preset der Liste zufügen
  };
}  // namespace bose_soundtoch_lib
#endif  // WSPRESETUPDATEOBJECT_HPP
