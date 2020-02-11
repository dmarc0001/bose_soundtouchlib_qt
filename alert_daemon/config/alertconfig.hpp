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
    private:
    // statische defaults
    const static char *defaultLogFile;
    const static char *keyLogFile;
    const static char *defaultLogPath;
    const static char *defaultBindAddr;
    const static char *keyBindAddr;
    const static char *defaultBindPort;
    const static char *keyBindPort;
    const static char *defaultWsport;
    const static char *keyWsport;
    const static char *defaultHttpPort;
    const static char *keyHttpPort;
    const static char *defaultConfigFile;
    // configfile
    const static char *constNoData;
    const static char *constLogGroupName;
    const static char *constKeyLogPath;
    const static char *constLogfilePathKey;
    const static char *constAppGroupName;
    const static char *constAlertGroupPattern;
    // aktuelle Config
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
    bool loadSettings( void );
    //! lade Einstellungen aus benannter Konfigdatei
    bool loadSettings( const QString &configFile );
    //! sichere Einstellungen
    bool saveSettings( void );
    //! lese logger einstellungen
    bool loadLogSettings( QSettings &settings );
    //
    //! Name der Konfigdatei ausgeben
    QString getConfigFile( void ) const;
    //! Name der Logdatei ausgeben
    QString getLogfileName( void ) const;
    // Pfad zur Logdatei zurück geben
    QString getLogfilePath( void ) const;
    //! Name der Logdatei setzten
    void setLogfileName( const QString &value );
    //! Name des Pfades zur Logdatei setzten
    void setLogFilePath( const QString &value );
    //! den vollen Pfas/Namen der logdatei...
    QString getFullLogFilePath( void );

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
    bool loadLogSettings( QSettings &settings );
    void makeDefaultLogSettings( QSettings &settings );
    bool saveLogSettings( QSettings &settings );
    // allg. Programmeinstellungen
    bool loadAppSettings( QSettings &settings );
    void makeAppDefaultSettings( QSettings &settings );
    bool saveAppSettings( QSettings &settings );
    // Alarm-einstellungen
  };
}  // namespace bose_commserver
#endif  // ALERTCONFIG_HPP
