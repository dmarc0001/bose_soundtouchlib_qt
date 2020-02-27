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

  enum LgMod : qint8
  {
    endl = 0,
    currDate
  };

  // um das schreiben abzukürzen
  constexpr LgThreshold LNONE = LgThreshold::LG_NONE;
  constexpr LgThreshold LDEBUG = LgThreshold::LG_DEBUG;
  constexpr LgThreshold LINFO = LgThreshold::LG_INFO;
  constexpr LgThreshold LWARN = LgThreshold::LG_WARN;
  constexpr LgThreshold LCRIT = LgThreshold::LG_CRIT;

  class Logger
  {
    private:
    //! Logs level
    LgThreshold threshold;
    LgThreshold currentThreshold;
    //! Zeiger auf das Logdateiobjekt
    std::unique_ptr< QFile > logFile;
    //! Zeiger auf einen Textstrom
    std::unique_ptr< QTextStream > textStream;
    //! das lokale Datum/Zeit objekt
    QDateTime dateTime;
    //! Mutex zum locken der Queue
    QMutex logMutex;
    //! Format der Zeitausgabe
    static const QString dateTimeFormat;
    //! String für keinen level
    static const QString NONE_STR;
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
    // Logger( const Logger &lg );
    virtual ~Logger();  //! Destruktor

    int startLogging( LgThreshold th = LG_DEBUG, const QString &fn = "logging.log" );  //! Loggen beginnen
    //! Setzte Loggingstufe
    void setThreshold( LgThreshold th );
    //! für << flag setzten
    inline void setCurrentThreshold( LgThreshold th )
    {
      currentThreshold = th;
    };
    inline LgThreshold getCurrentThreshold()
    {
      return currentThreshold;
    }
    //! lese Loggingstufe
    LgThreshold getThreshold( void );

    //
    // die Ausgaben als templates machen
    //
    template < typename T >
    void debug( const T msg )
    {
      //! Serialisieren...
      QMutexLocker locker( &logMutex );
      qDebug().noquote().nospace() << DEBUG_STR << msg;
      // if ( textStream && threshold >= LG_DEBUG )
      {
        *textStream << getDateString() << DEBUG_STR << msg << endl;
      }
    }
    template < typename T >
    void debug2( const T msg )
    {
      //! Serialisieren...
      QMutexLocker locker( &logMutex );
      qDebug().noquote().nospace() << msg;
      // if ( textStream && threshold >= LG_DEBUG )
      {
        *textStream << msg;
        /*
        Qt::endl()
        */
      }
    }

    template < typename T >
    void info( const T msg )
    {
      //! Serialisieren...
      QMutexLocker locker( &logMutex );
      qDebug().noquote().nospace() << INFO_STR << msg;
      // if ( textStream && threshold >= LG_INFO )
      {
        *textStream << getDateString() << INFO_STR << msg << endl;
      }
    }
    template < typename T >
    void info2( const T msg )
    {
      //! Serialisieren...
      QMutexLocker locker( &logMutex );
      qDebug().noquote().nospace() << msg;
      // if ( textStream && threshold >= LG_INFO )
      {
        *textStream << msg;
      }
    }

    template < typename T >
    void warn( const T msg )
    {
      //! Serialisieren...
      QMutexLocker locker( &logMutex );
      qDebug().noquote().nospace() << WARN_STR << msg;
      // if ( textStream && threshold >= LG_WARN )
      {
        *textStream << getDateString() << WARN_STR << msg << endl;
      }
    }
    template < typename T >
    void warn2( const T msg )
    {
      //! Serialisieren...
      QMutexLocker locker( &logMutex );
      qDebug().noquote().nospace() << msg;
      // if ( textStream && threshold >= LG_WARN )
      {
        *textStream << msg;
      }
    }

    template < typename T >
    void crit( const T msg )
    {
      //! Serialisieren...
      QMutexLocker locker( &logMutex );
      qDebug().noquote().nospace() << CRIT_STR << msg;
      // if ( textStream && threshold >= LG_CRIT )
      {
        *textStream << getDateString() << CRIT_STR << msg << endl;
      }
    }
    template < typename T >
    void crit2( const T msg )
    {
      //! Serialisieren...
      QMutexLocker locker( &logMutex );
      qDebug().noquote().nospace() << msg;
      // if ( textStream && threshold >= LG_CRIT )
      {
        *textStream << msg;
      }
    }

    //! der Spezialfall
    friend Logger &operator<<( Logger &lg, const LgThreshold th );
    friend Logger &operator<<( Logger &lg, const LgMod mod );
    // friend Logger &operator<<( Logger &lg, typeof( Qt::endl ) a );
    // Qt::endl

    template < typename T >
    friend Logger &operator<<( Logger &lg, const T msg )
    {
      switch ( lg.getCurrentThreshold() )
      {
        case LNONE:
          break;
        case LDEBUG:
          lg.debug2( msg );
          break;
        case LINFO:
          lg.info2( msg );
          break;
        case LWARN:
          lg.warn2( msg );
          break;
        case LCRIT:
          lg.crit2( msg );
          break;
      }
      return lg;
    }
    void lineEnd();
    void printCurrDateStr();
    //! Logger explizit herunterfahren
    void shutdown();
    //! string zum level erfahren
    static const QString getThresholdString( LgThreshold th );
    static LgThreshold getThresholdFromString( const QString &th );

    private:
    QString getDateString();  //! interne Funktion für den Datumsstring
  };
}  // namespace bose_commserver
#endif /* SRC_LOGGING_LOGGER_HPP_ */
