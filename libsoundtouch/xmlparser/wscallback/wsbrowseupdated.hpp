#ifndef WSBROWSEUPDATED_HPP
#define WSBROWSEUPDATED_HPP

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
  class SOUNDTOUCH_QT_LIBSHARED_EXPORT WsBrowseUpdated : public IResponseObject
  {
    private:
    QString source;         //! browse source?
    QString sourceAccount;  //! account für source

    public:
    explicit WsBrowseUpdated( QDomElement *domElem, QObject *parent );
    ~WsBrowseUpdated() override;
    //
    // GETTER
    //
    QString getSource() const;
    QString getSourceAccount() const;
  };
}  // namespace bose_soundtoch_lib

#endif  // WSBROWSEUPDATED_HPP
