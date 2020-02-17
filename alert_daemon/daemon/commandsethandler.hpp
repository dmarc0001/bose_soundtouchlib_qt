#ifndef COMMANDSETHANDLER_HPP
#define COMMANDSETHANDLER_HPP

#include <QDate>
#include <QJsonArray>
#include <QJsonObject>
#include <QTime>
#include "config/alertconfig.hpp"
#include "config/common_def.hpp"
#include "connectionhandler.hpp"

namespace bose_commserver
{
  // static const constexpr char NONE_AL[] = "not-a-alert";
  //
  class CommandSetHandler
  {
    private:
    AppConfigPtr config;
    QJsonObject gRequest;
    JSonStringPtr gAnswer;
    LoggerPtr lg;

    public:
    explicit CommandSetHandler( AppConfigPtr dconfig, QJsonObject request );
    JSonStringPtr getResponse();

    private:
    void insertOkFor( JsonObjSPtr jsonObj, const QString &key, bool isOk );
    SingleAlertConfig &findAlert( const QString &key );
    void updateSingleAlert( SingleAlertConfig &sAlert, const QVariantMap &map );
  };
}  // namespace bose_commserver
#endif  // COMMANDSETHANDLER_HPP
