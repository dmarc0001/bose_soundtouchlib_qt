#ifndef COMMANDGETHANDLER_HPP
#define COMMANDGETHANDLER_HPP

#include <QJsonArray>
#include <QJsonObject>
#include "common_def.hpp"
#include "config/alertconfig.hpp"

namespace bose_commserver
{
  class CommandGetHandler
  {
    private:
    AppConfigPtr config;
    QJsonObject gRequest;
    JSonStringPtr gAnswer;
    LoggerPtr lg;

    public:
    explicit CommandGetHandler( AppConfigPtr dconfig, QJsonObject request );
    JSonStringPtr getResponse();

    private:
    JSonStringPtr getJSONErrorMessage( const QString &errormsg );
    void insertDaemonConfig( JsonObjSPtr jsonObj );
    void insertAlertConfig( JsonObjSPtr jsonObj, const SingleAlertConfig &alert );
    void insertAvailDevices( JsonObjSPtr jsonObj );
  };
}  // namespace bose_commserver
#endif  // COMMANDGETHANDLER_HPP
