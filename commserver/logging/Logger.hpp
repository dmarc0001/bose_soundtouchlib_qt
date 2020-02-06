#ifndef SRC_LOGGING_LOGGER_HPP_
#define SRC_LOGGING_LOGGER_HPP_
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <QString>
#include <QTextStream>
#include <QtGlobal>
#include <memory>
#include <string>

namespace bose_commserver
{
  enum LgThreshold : quint8
  {
    LG_NONE = 0,
    LG_CRIT,  // 1
    LG_WARN,  // 2
    LG_INFO,  // 3
    LG_DEBUG  // 4
  };

  class Logger
  {
    private:
    //! Logs level
    LgThreshold threshold;
    //! Zeiger auf das Logdateiobjekt
    std::shared_ptr< QFile > logFile;
    //! Zeiger auf einen Textstrom
    std::shared_ptr< QTextStream > textStream;
    //! das lokale Datum/Zeit objekt
    QDateTime dateTime;
    //! Mutex zum locken der Queue
    std::shared_ptr< QMutex > logMutex;
    //! Format der Zeitausgabe
    static const QString dateTimeFormat;
    //! String für Debuglevel
    static const QString DEBUG_STR;
    //! String für Infolevel
    static const QString INFO_STR;
    //! String für Warnlevel
    static const QString WARN_STR;
    //! String für Kritischen level
    static const QString CRIT_STR;

    public:
    explicit Logger();
    Logger::Logger( Logger &lg );
    virtual ~Logger();  //! Destruktor

    int startLogging( LgThreshold th = LG_DEBUG, const QString &fn = "logging.log" );  //! Loggen beginnen
    //! Setzte Loggingstufe
    void setThreshold( LgThreshold th );
    //! lese Loggingstufe
    LgThreshold getThreshold( void );
    //
    //! INFO Ausgaben
    void info( const QString &msg );
    //! INFO Ausgaben
    void info( const std::string &msg );
    //! INFO Ausgaben
    void info( const char *msg );
    //
    //! DEBUG Ausgaben
    void debug( const QString &msg );
    //! DEBUG Ausgaben
    void debug( const std::string &msg );
    //! DEBUG Ausgaben
    void debug( const char *msg );
    //
    //! WARN Ausgaben
    void warn( const QString &msg );
    //! WARN Ausgaben
    void warn( const std::string &msg );
    //! WARN Ausgaben
    void warn( const char *msg );
    //
    //! CRIT Ausgaben
    void crit( const QString &msg );
    //! CRIT Ausgaben
    void crit( const std::string &msg );
    //! CRIT Ausgaben
    void crit( const char *msg );
    //
    void shutdown();  //! Logger explizit herunterfahren

    protected:
    std::shared_ptr< QFile > getLogFile();
    std::shared_ptr< QTextStream > getTextStream();

    private:
    QString getDateString();  //! interne Funktion für den Datumsstring
  };
}  // namespace bose_commserver
#endif /* SRC_LOGGING_LOGGER_HPP_ */
