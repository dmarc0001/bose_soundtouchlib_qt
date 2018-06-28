#ifndef WSLANGUAGEUPDATED_HPP
#define WSLANGUAGEUPDATED_HPP

#include <qglobal.h>
#include <QObject>
#include <QString>
#include <QtDebug>
#include <memory>
#include "bsoundtouch_global.hpp"
#include "iresponseobject.hpp"

namespace bose_soundtoch_lib
{
  class WsLanguageUpdated : public IResponseObject
  {
    private:
    QString language;  //! zu welcher sprache gewechselt?

    public:
    explicit WsLanguageUpdated( QXmlStreamReader *xmlreader, QObject *parent );
    ~WsLanguageUpdated() override;
    //
    // GETTER
    //
    QString getLanguage() const;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSLANGUAGEUPDATED_HPP
