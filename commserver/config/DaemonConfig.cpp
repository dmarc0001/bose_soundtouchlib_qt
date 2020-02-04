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

  DaemonConfig::DaemonConfig()
      : logfile( ProgramConfig::logfile )
      , bindaddr( ProgramConfig::bindaddr )
      , bindport( ProgramConfig::bindport )
      , defaultWsport( ProgramConfig::wsport )
      , defaultHttpport( ProgramConfig::httpport )
      , isDebug( false )

  {
  }
}  // namespace bose_commserver
