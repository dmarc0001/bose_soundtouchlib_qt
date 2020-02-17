#ifndef COMMON_DEF_HPP
#define COMMON_DEF_HPP

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include "config/alertconfig.hpp"

namespace bose_commserver
{
  // vorabdeklaration
  class AlertAppConfig;
  class Logger;
  //
  // Hilfestellung für einfacheres lesen
  //
  using JsonObjSPtr = std::shared_ptr< QJsonObject >;
  using JSonStringPtr = std::shared_ptr< QString >;
  using AppConfigPtr = std::shared_ptr< AlertAppConfig >;
  using LoggerPtr = std::shared_ptr< Logger >;
  //
  // der Timer, der alles steuert
  //
  constexpr int TIMER_INTERVAL = 1000;

  class command
  {
    public:
    // Kommandos
    static const constexpr char *setCmd{"set"};
    static const constexpr char *getCmd{"set"};
    static const constexpr char *deleteCmd{"delete"};

    // get subkommandos
    static const constexpr char *getConfigCmd{"config"};
    static const constexpr char *getConfigDaemon{"daemon"};
    static const constexpr char *getCongigAllCmd{"all"};
    static const constexpr char *getConfigDevCmd{"devices"};
    static const constexpr char *getConfigNewAlCmd{"new-alert"};

    // set subkommandos
    static const constexpr char *setConfigCmd{"config"};
    static const constexpr char *setConfigDaemon{"daemon"};
    static const constexpr char *setConfigDevCmd{"devices"};
    static const constexpr char *setConfigNewAlCmd{"new-alert"};

    // app konfig json namen
    static const constexpr char *confAppLogfile{"logfilename"};
    static const constexpr char *confAppConfigFile{"configfilename"};
    static const constexpr char *confAppBindAddr{"bindaddr"};
    static const constexpr char *confAppBindPort{"bindport"};
    static const constexpr char *confAppWsPort{"wsport"};
    static const constexpr char *confAppHttpPort{"httpport"};
    static const constexpr char *confAppThreshold{"threshold"};
    static const constexpr char *confAppDebug{"debug"};

    // alert config
    static const constexpr char *confAlertVolume{"volume"};
    static const constexpr char *confAlertDate{"date"};
    static const constexpr char *confAlertTime{"time"};
    static const constexpr char *confAlertLocation{"location"};
    static const constexpr char *confAlertSource{"source"};
    static const constexpr char *confAlertRaiseVol{"raise_vol"};
    static const constexpr char *confAlertDuration{"duration"};
    static const constexpr char *confAlertSourceAccount{"source_account"};
    static const constexpr char *confAlertDays{"days"};
    static const constexpr char *confAlertDevices{"devices"};
    static const constexpr char *confAlertType{"type"};
    static const constexpr char *confAlertEnabled{"enabled"};
    static const constexpr char *confAlertNote{"note"};
    static const constexpr char *confAlertNONE{"not-a-alert"};

    // Wochentage in der config
    static char const *const weekdays[ 8 ];

    // Zeit oder Datumskonstanten
    static const constexpr char *dateFormatStr{"yyyy-MM-dd"};
    static const constexpr char *timeFormatStr{"HH:mm"};
  };

#ifdef DEBUG
  //
  // das Format "aufgehübscht" (0)
  //
  constexpr QJsonDocument::JsonFormat jsonFormat = QJsonDocument::Indented;
#else
  //
  // das Format platzsparend (1)
  //
  constexpr QJsonDocument::JsonFormat jsonFormat = QJsonDocument::Compact;
#endif

}  // namespace bose_commserver

#endif
