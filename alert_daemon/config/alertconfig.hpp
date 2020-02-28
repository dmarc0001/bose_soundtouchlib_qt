#ifndef ALERTCONFIG_HPP
#define ALERTCONFIG_HPP

#include <QByteArray>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QRegularExpression>
#include <QSettings>
#include <QStandardPaths>
#include <QString>
#include <QTime>
#include <QTimer>
#include <QVector>
#include <memory>

#include "config/common_def.hpp"
#include "logging/logger.hpp"
#include "soundtouchdevice.hpp"

namespace bose_commserver
{
  //
  // Vorausdefinition
  //
  class SingleAlertConfig;
  using AlertList = QList< SingleAlertConfig >;
  using AlertListPtr = std::shared_ptr< AlertList >;

  //! Klasse beinhaltet die Konfiguration
  class AlertAppConfig : public QObject
  {
    Q_OBJECT
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
    static constexpr const char *constLogToConsoleKey{"consolelog"};
    // app einstellungen
    static constexpr const char *constAppGroupName{"application"};
    static constexpr const char *constBindAddrKey{"bindaddr"};
    static constexpr const char *constBindPortKey{"bindport"};
    static constexpr const char *constWsportKey{"wsport"};
    static constexpr const char *constHttpPortKey{"httpport"};
    // Alarm Einstellungen
    static constexpr const char *constAlertGroupPattern{"alert-"};
    static constexpr const char *constAlertVolumeKey{"volume"};
    static constexpr const char *constAlertDateKey{"date"};
    static constexpr const char *constAlertTimeKey{"time"};
    static constexpr const char *constAlertLocationKey{"location"};
    static constexpr const char *constAlertSourceKey{"source"};
    static constexpr const char *constAlertRaiseVolKey{"raise_vol"};
    static constexpr const char *constAlertDurationKey{"duration"};
    static constexpr const char *constAlertAccountKey{"source_account"};
    static constexpr const char *constAlertDaysKey{"days"};
    static constexpr const char *constAlertDevicesKey{"devices"};
    static constexpr const char *constAlertTypeKey{"type"};
    static constexpr const char *constAlertEnableKey{"enable"};
    static constexpr const char *constAlertNoteKey{"note"};
    // Suchpattern
    static constexpr const char *constPatternTime{"^([01][0-9])|(2[0-4]):[0-5][0-9]$"};

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
    bool haveToCreateConfigFile;
    bool logToConsole;
    bool isHashValid;
    std::shared_ptr< Logger > lg;
    //! pointer auf Liste mit Alarmmeldungen zum weitergeben
    //! und bearbeiten
    AlertListPtr alConfigs;
    QByteArray configHash;
    QTimer configCheckTimer;
    qint8 configTimerCounter;
    qint8 configSaveTimeout;
    SoundTouchDevicesList availDevices;

    public:
    //! der Konstruktor
    explicit AlertAppConfig( QObject *parent = nullptr );
    //! Destruktor
    virtual ~AlertAppConfig();
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
    //! gib einien hash über die config zurück und erstele den wenn nötig
    QByteArray getConfigHash();
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
    AlertListPtr getAlConfigs() const;
    QString getDefaultWsPort() const;
    QString getDefaultHttpPort() const;
    QRegularExpression getAlertPattern();
    QString getAlertPatternStr();
    SoundTouchDevicesList &getAvailDevices();
    void addAvailDevices( const SoundTouchDevice &dev );
    bool getLogToConsole() const;
    void setLogToConsole( bool value );

    private:
    void onConfigCheckTimer();
    // Logeinstellungen
    //! erzeuge default Einträge für logging
    void makeDefaultLogSettings( QSettings &settings );
    //! lese logger einstellungen
    bool loadLogSettings( QSettings &settings );
    //! lese allgemeine Einstellugnen der App
    bool loadAppSettings( QSettings &settings );
    void makeAppDefaultSettings( QSettings &settings );
    //! lade Einstellungen für spezifischen Alarm
    bool loadAlertSetting( QSettings &settings, const QString &currentAlertGroup );
    //! Datei mit Datumserweiterung kopieren
    QString duplicateConfigFile( const QString &fileName );
    bool resetConfigFile( const QString &duplicatFileName );
    bool saveLogSettings( QSettings &settings );
    bool saveAppSettings( QSettings &settings );
    bool saveAlertSettings( QSettings &settings, const SingleAlertConfig &currConfig );
  };

  //! Klasse beinhaltet Konfig für einnen einzelnen alarm
  class SingleAlertConfig
  {
    private:
    //
    // die Eigenschaften eines Alarms
    //
    QString alName;
    qint8 alVolume;
    QDate alDate;
    QTime alTime;
    QString alLocation;
    QString alSource;
    bool alRaiseVolume;
    qint16 alDuration;
    QString alSourceAccount;
    QList< qint8 > alDays;
    QStringList alDevices;
    QString alType;
    bool alEnable;
    QString alNote;
    bool isHashValid;
    QByteArray alertHash;  // nicht in der config speichern
    QDateTime runSince;    // nicht in der config speichern, nicht im hash

    public:
    explicit SingleAlertConfig( const QString &_name );
    //! gib einien hash über die config zurück und erstele den wenn nötig
    QByteArray getConfigHash();
    QString getName() const;
    qint8 getAlVolume() const;
    void setAlVolume( const qint8 &value );
    QDate getAlDate() const;
    void setAlDate( const QDate &value );
    QTime getAlTime() const;
    void setAlTime( const QTime &value );
    QString getAlLocation() const;
    void setAlLocation( const QString &value );
    QString getAlSource() const;
    void setAlSource( const QString &value );
    bool getAlRaiseVolume() const;
    void setAlRaiseVolume( bool value );
    qint16 getAlDuration() const;
    void setAlDuration( const qint16 &value );
    QString getAlSourceAccount() const;
    void setAlSourceAccount( const QString &value );
    QList< qint8 > getAlDays() const;
    QStringList getAlDaysStr() const;
    void setAlDays( const QList< qint8 > &value );
    QStringList getAlDevices() const;
    void setAlDevices( const QStringList &value );
    QString getAlType() const;
    void setAlType( const QString &value );
    bool getAlEnable() const;
    void setAlEnable( bool value );
    QString getAlNote() const;
    void setAlNote( const QString &value );
    QDateTime getRunSince() const;
    void setRunSince( const QDateTime &value );
  };
}  // namespace bose_commserver
#endif  // ALERTCONFIG_HPP
