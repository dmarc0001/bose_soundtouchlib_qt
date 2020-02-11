#include "alertconfig.hpp"

#include <QDebug>

namespace bose_commserver
{
  AlertConfig::AlertConfig()
      : logFileName( AlertConfig::defaultLogFile )
      , bindaddr( AlertConfig::defaultBindAddr )
      , bindport( AlertConfig::defaultBindPort )
      , wsPort( AlertConfig::defaultWsport )
      , httpPort( AlertConfig::defaultHttpPort )
      , logPath( AlertConfig::defaultLogPath )
      , threshold( AlertConfig::defaultThreshold )
      , configFileName( QString( "./%1.ini" ).arg( QCoreApplication::applicationName() ) )
      , isDebug( false )
      , isDebugManual( false )
      , isLogfileManual( false )
      , isBindAddrManual( false )
      , isBindPortManual( false )
      , lg( nullptr )
  {
    qDebug().noquote() << "AlertConfig::AlertConfig()";
    qDebug().noquote() << "AlertConfig::AlertConfig(): TEST config file: " << configFileName;
    QFile iniFile( configFileName );
    if ( iniFile.exists() && iniFile.isWritable() )
    {
      // Alles OK
      qDebug().noquote() << "AlertConfig::AlertConfig(): config file: " << configFileName;
    }
    /*
    else
    {
      configFileName = QStandardPaths::writableLocation( QStandardPaths::AppDataLocation )
                           .append( "/" )
                           .append( QCoreApplication::applicationName() )
                           .append( ".ini" );
      qDebug().noquote() << "AlertConfig::AlertConfig(): config file placeholder: " << configFileName;
    }
*/
  }

  AlertConfig::~AlertConfig()
  {
    qDebug().noquote() << "AlertConfig::~AlertConfig()";
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
  bool AlertConfig::loadSettings()
  {
    qDebug().noquote() << "AlertConfig::loadSettings(), logfile: " << getFullLogFilePath();
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
    //
    // suche alle möglichen alarme
    //
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
    qDebug().noquote() << "AlertConfig::loadSettings(" << configFile << ")...";
    configFileName = configFile;
    return loadSettings();
  }

  /**
   * @brief AlertConfig::loadLogSettings
   * @param settings
   * @return
   */
  bool AlertConfig::loadLogSettings( QSettings &settings )
  {
    bool retval = true;
    qDebug().noquote() << "AlertConfig::loadLogSettings() START...";
    //
    // Öffne die Gruppe Logeinstellungen
    //
    settings.beginGroup( AlertConfig::constLogGroupName );
    //
    // lese den Logpfad aus
    //
    logPath = settings.value( AlertConfig::constKeyLogPath, AlertConfig::constNoData ).toString();
    if ( QString::compare( logPath, AlertConfig::constNoData ) == 0 )
    {
      // Nicht gefunden -> Fehler markieren
      retval = false;
    }
    qDebug().noquote().nospace() << "AlertConfig::loadLogSettings(): Logfile Path: <" << logPath << ">";
    //
    // Lese den Dateinamen aus
    //
    logFileName = settings.value( AlertConfig::constKeyLogFile, AlertConfig::constNoData ).toString();
    if ( QString::compare( logFileName, AlertConfig::constNoData ) == 0 )
    {
      // Nicht gefunden -> Fehler markieren
      retval = false;
    }
    qDebug().noquote().nospace() << "AlertConfig::loadLogSettings(): Logfile: <" << logFileName << ">";
    //
    // lese loglevel aus
    //
    threshold = static_cast< LgThreshold >(
        settings.value( AlertConfig::constLogThresholdKey, static_cast< int >( LgThreshold::LG_NONE ) ).toInt() & 0xff );
    if ( threshold == LgThreshold::LG_NONE )
    {
      threshold = AlertConfig::defaultThreshold;
    }
    qDebug().noquote().nospace() << "AlertConfig::loadLogSettings(): loglevel: <" << Logger::getThresholdString( threshold ) << ">";
    //
    // Ende der Gruppe
    //
    settings.endGroup();
    //
    // Ergebnis kommunizieren
    //
    qDebug().noquote() << "AlertConfig::loadLogSettings() END...";
    return retval;
  }

  /**
   * @brief AlertConfig::makeDefaultLogSettings
   * @param settings
   */
  void AlertConfig::makeDefaultLogSettings( QSettings &settings )
  {
    qDebug().noquote() << "AlertConfig::makeDefaultLogSettings()";
    //
    // Öffne die Gruppe Logeinstellungen
    //
    settings.beginGroup( AlertConfig::constLogGroupName );
    //
    // defaultwerte setzten
    //
    logPath = QStandardPaths::writableLocation( QStandardPaths::AppDataLocation );
    settings.setValue( AlertConfig::constKeyLogPath, logPath );
    logFileName = QCoreApplication::applicationName().append( ".log" );
    settings.setValue( AlertConfig::constKeyLogFile, logFileName );
    threshold = AlertConfig::defaultThreshold;
    settings.setValue( AlertConfig::constLogThresholdKey, static_cast< int >( threshold ) );
    //
    // Ende der Gruppe
    //
    settings.endGroup();
  }

  bool AlertConfig::saveSettings( void )
  {
    return false;
  }

  bool AlertConfig::loadAppSettings( QSettings &settings )
  {
    bool retval = true;
    qDebug().noquote() << "AlertConfig::loadAppSettings() START...";
    //
    // Öffne die Gruppe app
    //
    settings.beginGroup( AlertConfig::constAppGroupName );
    //
    // Bindadresse des Dienstes / der app
    //
    bindaddr = settings.value( AlertConfig::constBindAddrKey, AlertConfig::defaultBindAddr ).toString();
    qDebug().noquote() << "AlertConfig::loadAppSettings(): bindaddr for app: " << bindaddr;
    //
    // Bindport für die App
    //
    bindport = settings.value( AlertConfig::constBindPortKey, AlertConfig::defaultBindPort ).toString();
    qDebug().noquote() << "AlertConfig::loadAppSettings(): bindport for app: " << bindport;
    //
    // default port for ws connect to devices
    //
    wsPort = settings.value( AlertConfig::constWsportKey, AlertConfig::defaultWsport ).toString();
    qDebug().noquote() << "AlertConfig::loadAppSettings(): dest ws port for devices: " << wsPort;
    //
    // default port for http connect to devices
    //
    httpPort = settings.value( AlertConfig::constHttpPortKey, AlertConfig::defaultHttpPort ).toString();
    qDebug().noquote() << "AlertConfig::loadAppSettings(): dest http port for devices: " << httpPort;
    //
    // Ende der Gruppe
    //
    settings.endGroup();
    //
    // Ergebnis kommunizieren
    //
    return ( retval );
  }

  void AlertConfig::makeAppDefaultSettings( QSettings &settings )
  {
    qDebug().noquote() << "AlertConfig::loadAppSettings() START...";
    //
    // Öffne die Gruppe app
    //
    settings.beginGroup( AlertConfig::constAppGroupName );
    //
    // default werte setzen
    //
    bindaddr = AlertConfig::defaultBindAddr;
    settings.setValue( AlertConfig::constBindAddrKey, bindaddr );
    bindport = AlertConfig::defaultBindPort;
    settings.setValue( AlertConfig::constBindPortKey, bindport );
    wsPort = AlertConfig::defaultWsport;
    settings.setValue( AlertConfig::constWsportKey, wsPort );
    httpPort = AlertConfig::defaultHttpPort;
    settings.setValue( AlertConfig::constHttpPortKey, httpPort );
    //
    // Gruppe schliessen
    //
    settings.endGroup();
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
    return logPath;
  }

  void AlertConfig::setLogfileName( const QString &value )
  {
    logFileName = value;
  }

  void AlertConfig::setLogFilePath( const QString &value )
  {
    logPath = value;
  }

  QString AlertConfig::getFullLogFilePath( void )
  {
    // um korrekte Pfade zu erzeugen ein QFileobjekt
    QDir lp( logPath );
    if ( lp.exists() && lp.isReadable() )
    {
      logPath = lp.dirName();
    }
    else
    {
      logPath = ".";
    }
    QFile logFile( QString().append( logPath ).append( "/" ).append( logFileName ) );
    // Fileinfo zur korrekten erzeugung des kompletten Pfdades
    QFileInfo logFileInfo( logFile );
    return ( logFileInfo.filePath() );
  }

}  // namespace bose_commserver
