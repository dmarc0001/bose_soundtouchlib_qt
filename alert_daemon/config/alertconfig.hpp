#ifndef ALERTCONFIG_HPP
#define ALERTCONFIG_HPP

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QSettings>
#include <QStandardPaths>
#include <QString>
#include <QVector>
#include <memory>

#include "logging/logger.hpp"

namespace bose_commserver
{
  class AlertConfig
  {
    protected:
    // statische defaults
    static constexpr const char *defaultLogFile{"alert_daemon.log"};
    static constexpr const char *defaultLogPath{""};
    static constexpr const char *defaultBindAddr{"0.0.0.0"};
    static constexpr const char *defaultBindPort{"8080"};
    static constexpr const char *defaultWsport{"8080"};
    static constexpr const char *defaultHttpPort{"8090"};
    static constexpr const LgThreshold defaultThreshold{LgThreshold::LG_CRIT};
    // globale festlegungen
    static constexpr const char *constNoData{"-"};
    // logfile festlegungen
    static constexpr const char *constLogGroupName{"logger"};
    static constexpr const char *constKeyLogPath{"logpath"};
    static constexpr const char *constKeyLogFile{"logfile"};
    static constexpr const char *constLogThresholdKey{"threshold"};
    // app einstellungen
    static constexpr const char *constAppGroupName{"application"};
    static constexpr const char *constBindAddrKey{"bindaddr"};
    static constexpr const char *constBindPortKey{"bindport"};
    static constexpr const char *constWsportKey{"wsport"};
    static constexpr const char *constHttpPortKey{"httpport"};
    // Alarm Einstellungen
    static constexpr const char *constAlertGroupPattern{"alert-"};
    // aktuelle Config
    private:
    QString logFileName;
    QString bindaddr;
    QString bindport;
    QString wsPort;
    QString httpPort;
    QString logPath;
    LgThreshold threshold;
    QString configFileName;
    bool isDebug;
    bool isDebugManual;
    bool isLogfileManual;
    bool isBindAddrManual;
    bool isBindPortManual;
    std::shared_ptr< Logger > lg;

    public:
    //! der Konstruktor
    explicit AlertConfig();
    //! Destruktor
    virtual ~AlertConfig();
    //! default logfilename
    static QString getDefaultLogFile();
    //! default binddir zurückgeben
    static QString getDefaultBindAddr();
    //! default bind port
    static QString getDefaultBindPort();
    //! lade Einstellungen aus default Konfigdatei
    bool loadSettings();
    //! lade Einstellungen aus benannter Konfigdatei
    bool loadSettings( const QString &configFile );
    //! sichere Einstellungen
    bool saveSettings();
    //
    //! Name der Konfigdatei ausgeben
    QString getConfigFile() const;
    //! Name der Logdatei ausgeben
    QString getLogfileName() const;
    // Pfad zur Logdatei zurück geben
    QString getLogfilePath() const;
    //! Name der Logdatei setzten
    void setLogfileName( const QString &value );
    //! Name des Pfades zur Logdatei setzten
    void setLogFilePath( const QString &value );
    //! den vollen Pfas/Namen der logdatei...
    QString getFullLogFilePath();

    bool getIsDebug() const;
    void setIsDebug( bool value );

    QString getLogFileName() const;
    void setLogFileName( const QString &value );

    QString getBindaddr() const;
    void setBindaddr( const QString &value );

    QString getBindport() const;
    void setBindport( const QString &value );

    LgThreshold getThreshold() const;
    void setThreshold( const LgThreshold &value );

    std::shared_ptr< Logger > getLogger() const;
    void setLogger( const std::shared_ptr< Logger > &value );

    private:
    // Logeinstellungen
    //! erzeuge default Einträge für logging
    void makeDefaultLogSettings( QSettings &settings );
    //! lese logger einstellungen
    bool loadLogSettings( QSettings &settings );
    //! sichere Log settings
    bool saveLogSettings( QSettings &settings );
    //! lese allgemeine Einstellugnen der App
    bool loadAppSettings( QSettings &settings );
    void makeAppDefaultSettings( QSettings &settings );
    bool saveAppSettings( QSettings &settings );
    // Alarm-einstellungen
  };
}  // namespace bose_commserver
#endif  // ALERTCONFIG_HPP
