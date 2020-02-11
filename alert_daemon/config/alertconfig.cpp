#include "alertconfig.hpp"
#include <QDebug>

namespace bose_commserver
{
  const char *AlertConfig::defaultLogFile{"alert_daemon.log"};
  const char *AlertConfig::keyLogFile{"logfile"};
  const char *AlertConfig::defaultLogPath{""};
  const char *AlertConfig::defaultBindAddr{"0.0.0.0"};
  const char *AlertConfig::keyBindAddr{"bindaddr"};
  const char *AlertConfig::defaultBindPort{"8080"};
  const char *AlertConfig::keyBindPort{"bindport"};
  const char *AlertConfig::defaultWsport{"8080"};
  const char *AlertConfig::keyWsport{"wsport"};
  const char *AlertConfig::defaultHttpPort{"8090"};
  const char *AlertConfig::keyHttpPort{"httpport"};
  const char *AlertConfig::defaultConfigFile{"alert_config.ini"};
  // logfile festlegungen
  const char *AlertConfig::constNoData{"-"};
  const char *AlertConfig::constLogGroupName{"logger"};
  const char *AlertConfig::constKeyLogPath{"logpath"};
  const char *AlertConfig::constAppGroupName{"application"};
  const char *AlertConfig::constAlertGroupPattern{"alert-"};

  AlertConfig::AlertConfig()
      : logFileName( AlertConfig::defaultLogFile )
      , bindaddr( AlertConfig::defaultBindAddr )
      , bindport( AlertConfig::defaultBindPort )
      , wsPort( AlertConfig::defaultWsport )
      , httpPort( AlertConfig::defaultHttpPort )
      , logPath( AlertConfig::defaultLogPath )
      , threshold( LgThreshold::LG_DEBUG )
      , configFileName( AlertConfig::defaultConfigFile )
      , isDebug( false )
      , isDebugManual( false )
      , isLogfileManual( false )
      , isBindAddrManual( false )
      , isBindPortManual( false )
      , lg( nullptr )
  {
    qDebug().noquote() << "AppConfigClass::AppConfigClass()";
    QFile iniFile( configFileName );
    if ( iniFile.exists() && iniFile.isWritable() )
    {
      // Alles OK
    }
    else
    {
      configFileName = QStandardPaths::writableLocation( QStandardPaths::AppDataLocation )
                           .append( "/" )
                           .append( QCoreApplication::applicationName() )
                           .append( ".ini" );
    }
  }

  AlertConfig::~AlertConfig()
  {
    qDebug().noquote() << "AppConfigClass::~AppConfigClass()";
    saveSettings();
  }

  QString AlertConfig::getDefaultLogFile()
  {
    return QString( AlertConfig::defaultLogFile );
  }

  QString AlertConfig::getDefaultBindAddr()
  {
    return QString( AlertConfig::defaultBindAddr );
  }

  QString AlertConfig::getDefaultBindPort()
  {
    return QString( AlertConfig::defaultBindPort );
  }

  QString AlertConfig::getLogFileName() const
  {
    return logFileName;
  }

  void AlertConfig::setLogFileName( const QString &value )
  {
    logFileName = value;
    isLogfileManual = true;
  }

  QString AlertConfig::getBindaddr() const
  {
    return bindaddr;
  }

  void AlertConfig::setBindaddr( const QString &value )
  {
    bindaddr = value;
    isBindAddrManual = true;
  }

  QString AlertConfig::getBindport() const
  {
    return bindport;
  }

  void AlertConfig::setBindport( const QString &value )
  {
    bindport = value;
    isBindPortManual = true;
  }

  LgThreshold AlertConfig::getThreshold() const
  {
    return threshold;
  }

  void AlertConfig::setThreshold( const LgThreshold &value )
  {
    threshold = value;
  }

  std::shared_ptr< Logger > AlertConfig::getLogger() const
  {
    return lg;
  }

  void AlertConfig::setLogger( const std::shared_ptr< Logger > &value )
  {
    lg = value;
  }

  /**
   * @brief AlertConfig::loadSettings
   * Lade Einstellungen aus der Datei
   * @return
   */
  bool AlertConfig::loadSettings( void )
  {
    qDebug().noquote() << "AppConfigClass::~loadSettings(), logfile: " << getFullLogFilePath();
    //
    // Ini Dateihandling initialisieren, Datei einlesen
    //
    QSettings settings( configFileName, QSettings::IniFormat );
    //
    // die verschiedenen Sektionen behandeln
    //
    if ( !loadLogSettings( settings ) )
    {
      makeDefaultLogSettings( settings );
    }
    if ( !loadAppSettings( settings ) )
    {
      makeAppDefaultSettings( settings );
    }

    QString alertGroupBody( AlertConfig::constAlertGroupPattern );
    for ( int i = 0; i < 100; i++ )
    {
      //
      // suche nach Alarm von 00 bis 99
      //
      QString currentAlertGroup = QString( "%1%2" ).arg( alertGroupBody ).arg( i, 2, 10, QLatin1Char( '0' ) );
      // TODO: loadAlertSetting( currentAlertGroup );
    }

    return ( true );
  }

  /**
   * @brief AlertConfig::loadSettings
   * Lade Einstellungen aus einer angegebenen Datei
   * @param configFile
   * @return
   */
  bool AlertConfig::loadSettings( const QString &configFile )
  {
    qDebug().noquote() << "AppConfigClass::~loadSettings(" << configFile << ")...";
    configFileName = configFile;
    return loadSettings();
  }

  bool AlertConfig::loadLogSettings( QSettings &settings )
  {
    bool retval = true;
    qDebug().noquote() << "AppConfigClass::~loadLogSettings() START...";
    //
    // Öffne die Gruppe Logeinstellungen
    //
    settings.beginGroup( constLogGroupName );
    //
    // lese den Logpfad aus
    //
    logPath = settings.value( AlertConfig::constKeyLogPath, AlertConfig::constNoData ).toString();
    if ( QString::compare( logfilePath, constNoData ) == 0 )
    {
      // Nicht gefunden -> Fehler markieren
      retval = false;
    }
    qDebug().noquote().nospace() << "AppConfigClass::loadLogSettings(): Logfile Path: <" << logfilePath << ">";
    //
    // Lese den Dateinamen aus
    //
    logfileName = settings.value( constLogFileKey, AppConfigClass::constNoData ).toString();
    if ( QString::compare( logfileName, constNoData ) == 0 )
    {
      // Nicht gefunden -> Fehler markieren
      retval = false;
    }
    qDebug().noquote().nospace() << "AppConfigClass::loadLogSettings(): Logfile: <" << logfileName << ">";
    //
    // Ende der Gruppe
    //
    settings.endGroup();
    //
    // Ergebnis kommunizieren
    //

    qDebug().noquote() << "AppConfigClass::~loadLogSettings() END...";
    return retval;
  }

  bool AlertConfig::saveSettings( void )
  {
    return false;
  }

  bool AlertConfig::getIsDebug() const
  {
    return isDebug;
  }

  void AlertConfig::setIsDebug( bool value )
  {
    isDebug = value;
    isDebugManual = true;
  }

  QString AlertConfig::getConfigFile() const
  {
    return ( configFileName );
  }

  QString AlertConfig::getLogfileName( void ) const
  {
    return logFileName;
  }

  QString AlertConfig::getLogfilePath( void ) const
  {
    return logpath;
  }

  void AlertConfig::setLogfileName( const QString &value )
  {
    logFileName = value;
  }

  void AlertConfig::setLogFilePath( const QString &value )
  {
    logpath = value;
  }

  QString AlertConfig::getFullLogFilePath( void )
  {
    // um korrekte Pfade zu erzeugen ein QFileobjekt
    QFile logFile( QString().append( logpath ).append( "/" ).append( logFileName ) );
    // Fileinfo zur korrekten erzeugung des kompletten Pfdades
    QFileInfo logFileInfo( logFile );
    return ( logFileInfo.filePath() );
  }

}  // namespace bose_commserver
