#ifndef DAEMONCONFIG_HPP
#define DAEMONCONFIG_HPP

#include <QObject>
#include <QString>

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
    bool isDebug;

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
  };
}  // namespace bose_commserver
#endif  // DAEMONCONFIG_HPP
