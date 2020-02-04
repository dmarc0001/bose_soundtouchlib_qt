#ifndef WSAUDIOPRODUCTLEVELCONTROLS_HPP
#define WSAUDIOPRODUCTLEVELCONTROLS_HPP

#include <qglobal.h>
#include <QDomElement>
#include <QDomNode>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "../bsoundtouch_global.hpp"
#include "../iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT WsAudioProductLevelControls : public IResponseObject
  {
    private:
    QString controls;  //! controls?

    public:
    explicit WsAudioProductLevelControls( QDomElement *domElem, QObject *parent );
    ~WsAudioProductLevelControls() override;
    //
    // GETTER
    //
    QString getControls() const;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSAUDIOPRODUCTLEVELCONTROLS_HPP
