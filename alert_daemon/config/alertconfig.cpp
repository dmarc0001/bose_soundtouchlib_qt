#include "alertconfig.hpp"

namespace bose_commserver
{
  const char *AlertConfig::defaultLogFile{"alert_daemon.log"};
  const char *AlertConfig::keyLogFile{"logfile"};
  const char *AlertConfig::defaultLogPath{""};
  const char *AlertConfig::keyLogPath{"logpath"};
  const char *AlertConfig::defaultBindAddr{"0.0.0.0"};
  const char *AlertConfig::keyBindAddr{"bindaddr"};
  const char *AlertConfig::defaultBindPort{"8080"};
  const char *AlertConfig::keyBindPort{"bindport"};
  const char *AlertConfig::defaultWsport{"8080"};
  const char *AlertConfig::keyWsport{"wsport"};
  const char *AlertConfig::defaultHttpPort{"8090"};
  const char *AlertConfig::keyHttpPort{"httpport"};
  const char *AlertConfig::defaultConfigFile{"alert_config.ini"};

  AlertConfig::AlertConfig()
      : logFileName( AlertConfig::defaultLogFile )
      , bindaddr( AlertConfig::defaultBindAddr )
      , bindport( AlertConfig::defaultBindPort )
      , wsPort( AlertConfig::defaultWsport )
      , httpPort( AlertConfig::defaultHttpPort )
      , logpath( AlertConfig::defaultLogPath )
      , threshold( LgThreshold::LG_DEBUG )
      , configFileName( AlertConfig::defaultConfigFile )
      , isDebug( false )
      , isDebugManual( false )
      , isLogfileManual( false )
      , isBindAddrManual( false )
      , isBindPortManual( false )
      , lg( nullptr )
  {
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

  bool AlertConfig::loadSettings( void )
  {
    return false;
  }

  bool AlertConfig::loadSettings( QString &configFile )
  {
    configFileName = configFile;
    return true;
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
