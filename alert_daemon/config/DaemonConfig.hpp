#ifndef DAEMONCONFIG_HPP
#define DAEMONCONFIG_HPP

#include <QObject>
#include <QString>
#include "logging/Logger.hpp"

namespace bose_commserver
{
  class DaemonConfig
  {
    private:
    QString logfile;
    QString bindaddr;
    QString bindport;
    QString defaultWsport;
    QString defaultHttpport;
    QString loggerFile;
    QString logpath;
    LgThreshold threshold;
    bool isDebug;
    //
    std::shared_ptr< Logger > lg;

    public:
    DaemonConfig();
    QString getLogfile() const;
    void setLogfile( const QString &value );
    QString getBindaddr() const;
    void setBindaddr( const QString &value );
    QString getBindport() const;
    void setBindport( const QString &value );
    QString getDefaultWsport() const;
    void setDefaultWsport( const QString &value );
    QString getDefaultHttpport() const;
    void setDefaultHttpport( const QString &value );
    bool getIsDebug() const;
    void setIsDebug( bool value );
    QString getLoggerFileName() const;
    void setLoggerFileName( const QString &value );
    LgThreshold getThreshold() const;
    void setThreshold( const LgThreshold &value );
    QString getLogpath() const;
    void setLogpath( const QString &value );
    std::shared_ptr< Logger > getLogger();
    void setLogger( const std::shared_ptr< Logger > value );
  };
}  // namespace bose_commserver
#endif  // DAEMONCONFIG_HPP
