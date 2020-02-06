#include "DaemonConfig.hpp"
#include "programconfig.hpp"

namespace bose_commserver
{
  QString DaemonConfig::getLogfile() const
  {
    return logfile;
  }

  void DaemonConfig::setLogfile( const QString &value )
  {
    logfile = value;
  }

  QString DaemonConfig::getBindaddr() const
  {
    return bindaddr;
  }

  void DaemonConfig::setBindaddr( const QString &value )
  {
    bindaddr = value;
  }

  QString DaemonConfig::getBindport() const
  {
    return bindport;
  }

  void DaemonConfig::setBindport( const QString &value )
  {
    bindport = value;
  }

  QString DaemonConfig::getDefaultWsport() const
  {
    return defaultWsport;
  }

  void DaemonConfig::setDefaultWsport( const QString &value )
  {
    defaultWsport = value;
  }

  QString DaemonConfig::getDefaultHttpport() const
  {
    return defaultHttpport;
  }

  void DaemonConfig::setDefaultHttpport( const QString &value )
  {
    defaultHttpport = value;
  }

  bool DaemonConfig::getIsDebug() const
  {
    return isDebug;
  }

  void DaemonConfig::setIsDebug( bool value )
  {
    isDebug = value;
  }

  QString DaemonConfig::getLogger() const
  {
    return logger;
  }

  void DaemonConfig::setLogger( const QString &value )
  {
    logger = value;
  }

  LgThreshold DaemonConfig::getThreshold() const
  {
    return threshold;
  }

  void DaemonConfig::setThreshold( const LgThreshold &value )
  {
    threshold = value;
  }

  QString DaemonConfig::getLogpath() const
  {
    return logpath;
  }

  void DaemonConfig::setLogpath( const QString &value )
  {
    logpath = value;
  }

  std::shared_ptr< Logger > DaemonConfig::getLogger()
  {
    return lg;
  }

  void DaemonConfig::setLogger( const std::shared_ptr< Logger > &value )
  {
    lg = value;
  }

  DaemonConfig::DaemonConfig()
      : logfile( ProgramConfig::logfile )
      , bindaddr( ProgramConfig::bindaddr )
      , bindport( ProgramConfig::bindport )
      , defaultWsport( ProgramConfig::wsport )
      , defaultHttpport( ProgramConfig::httpport )
      , logger( ProgramConfig::logfile )
      , logpath( ProgramConfig::logpath )
      , threshold( ProgramConfig::threshold )
      , isDebug( false )
      , lg( nullptr )
  {
  }
}  // namespace bose_commserver
