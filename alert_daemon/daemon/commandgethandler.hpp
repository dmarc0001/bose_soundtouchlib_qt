#ifndef COMMANDGETHANDLER_HPP
#define COMMANDGETHANDLER_HPP

#include <QJsonArray>
#include <QJsonObject>

#include "config/alertconfig.hpp"
#include "config/common_def.hpp"
#include "connectionhandler.hpp"

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
    void insertDaemonConfig( JsonObjSPtr jsonObj );
    void insertAlertConfig( JsonObjSPtr jsonObj, const SingleAlertConfig &alert );
    void insertAvailDevices( JsonObjSPtr jsonObj );
    void insertNewAlertNum( JsonObjSPtr jsonObj, int num );
  };
}  // namespace bose_commserver
#endif  // COMMANDGETHANDLER_HPP
