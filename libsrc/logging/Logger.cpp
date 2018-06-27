#include "Logger.hpp"
#include <QDateTime>
#include <QFileInfo>

namespace bose_soundtoch_lib
{
  const QString Logger::dateTimeFormat = "[yyyy-MM-dd hh:mm:ss.z] ";
  const QString Logger::DEBUG_STR = "DEBUG ";
  const QString Logger::INFO_STR = "INFO  ";
  const QString Logger::WARN_STR = "WARN  ";
  const QString Logger::CRIT_STR = "CRIT  ";

  /**
   * @brief Logger::Logger Konstruktor mit Konfigurationsdatei Übergabe
   * @param lFile
   */
  Logger::Logger() : threshold( LG_DEBUG ), logFile( nullptr ), textStream( nullptr ), isTextStream( false )
  {
  }

  Logger::~Logger()
  {
    shutdown();
  }

  int Logger::startLogging( LgThreshold th, QString fn )
  {
    threshold = th;
    qInstallMessageHandler( &Logger::myMessageOutput );
    //
    // gibt es einen Lognamen
    //
    if ( fn != nullptr )
    {
      if ( fn.length() > 4 )
      {
        // Super, das Logfile ist benannt!
        qDebug().noquote().nospace() << "START LOGGING...";
        logFile = std::unique_ptr< QFile >( new QFile( fn ) );
        logFile->open( QIODevice::WriteOnly | QIODevice::Append );
        textStream = std::unique_ptr< QTextStream >( new QTextStream( logFile.get() ) );
        *textStream << getDateString() << "START LOGGING" << endl;
        //*textStream << getDateString() << "logfile exist? " << logFile->exists() << endl;
        //*textStream << getDateString() << "logfile opened? " << logFile->isOpen() << endl;
        //*textStream << getDateString() << "textStream status? " << ( textStream->status() == QTextStream::Ok ) << endl;
        if ( logFile->exists() && logFile->isOpen() && textStream->status() == QTextStream::Ok )
        {
          isTextStream = true;
          qDebug().noquote().nospace() << "START LOGGING...OK";
          *textStream << getDateString() << "START LOGGING...OK" << endl;
          return ( 1 );
        }
      }
    }
    //
    // Oh, da lief was falsch, LOGGEN nicht möglich
    //
    qCritical().noquote().nospace() << "START LOGGING...FAILED";
    return ( 0 );
  }

  /**
   * @brief Logger::setThreshold
   * @param th
   */
  void Logger::setThreshold( LgThreshold th )
  {
    threshold = th;
  }

  /**
   * @brief Logger::getThreshold
   * @return
   */
  LgThreshold Logger::getThreshold( void )
  {
    return ( threshold );
  }

  /**
   * @brief Logger::myMessageOutput
   * @param type
   * @param context
   * @param msg
   */
  void Logger::myMessageOutput( QtMsgType type, const QMessageLogContext &context, const QString &msg )
  {
    QByteArray localMsg = msg.toLocal8Bit();
    switch ( type )
    {
      case QtDebugMsg:
        fprintf( stdout, "DEBUG: %s (%s)\n", localMsg.constData(), context.function );
        // fprintf( stdout, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
      case QtInfoMsg:
        fprintf( stdout, "INFO: %s\n", localMsg.constData() );
        // fprintf( stdout, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
      case QtWarningMsg:
        fprintf( stdout, "WARNING: %s\n", localMsg.constData() );
        // fprintf( stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
      case QtCriticalMsg:
        fprintf( stdout, "CRITICAL: %s (%s:%u)\n", localMsg.constData(), context.file, context.line );
        // fprintf( stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
      case QtFatalMsg:
        fprintf( stdout, "FATAL: %s (%s:%u)\n", localMsg.constData(), context.file, context.line );
        // fprintf( stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
    }
  }

  /**
   * @brief Logger::warn Ausgabe(n) für WARNUNG
   * @param msg
   */
  void Logger::warn( const QString &msg )
  {
    qWarning().noquote().nospace() << msg;
    if ( isTextStream && ( threshold >= LG_WARN ) )
    {
      *textStream << getDateString() << WARN_STR << msg << endl;
    }
  }

  void Logger::warn( const char *msg )
  {
    qWarning().noquote().nospace() << msg;
    if ( isTextStream && ( threshold >= LG_WARN ) )
    {
      *textStream << getDateString() << WARN_STR << msg << endl;
    }
  }

  void Logger::warn( const std::string &msg )
  {
    qWarning().noquote().nospace() << msg.c_str();
    if ( isTextStream && ( threshold >= LG_WARN ) )
    {
      *textStream << getDateString() << WARN_STR << msg.c_str() << endl;
    }
  }

  /**
   * @brief Logger::info Ausgabe(n) für Info
   * @param msg
   */
  void Logger::info( const QString &msg )
  {
    qInfo().noquote().nospace() << msg;
    if ( isTextStream && ( threshold >= LG_INFO ) )
    {
      *textStream << getDateString() << INFO_STR << msg << endl;
    }
  }

  void Logger::info( const char *msg )
  {
    qInfo().noquote().nospace() << msg;
    if ( isTextStream && ( threshold >= LG_INFO ) )
    {
      *textStream << getDateString() << INFO_STR << msg << endl;
    }
  }
  void Logger::info( const std::string &msg )
  {
    qInfo().noquote().nospace() << msg.c_str();
    if ( isTextStream && ( threshold >= LG_INFO ) )
    {
      *textStream << getDateString() << INFO_STR << msg.c_str() << endl;
    }
  }

  /**
   * @brief Logger::debug Ausgaben für Debugging
   * @param msg
   */
  void Logger::debug( const QString &msg )
  {
    qDebug().noquote().nospace() << msg;
    if ( isTextStream && ( threshold >= LG_DEBUG ) )
    {
      *textStream << getDateString() << DEBUG_STR << msg << endl;
    }
  }

  void Logger::debug( const char *msg )
  {
    qDebug().noquote().nospace() << msg;
    if ( isTextStream && ( threshold >= LG_DEBUG ) )
    {
      *textStream << getDateString() << DEBUG_STR << msg << endl;
    }
  }

  void Logger::debug( const std::string &msg )
  {
    qDebug().noquote().nospace() << msg.c_str();
    if ( isTextStream && ( threshold >= LG_DEBUG ) )
    {
      *textStream << getDateString() << DEBUG_STR << msg.c_str() << endl;
    }
  }

  /**
   * @brief Logger::crit Ausgaben für Kritische Fehler
   * @param msg
   */
  void Logger::crit( const QString &msg )
  {
    qCritical().noquote().nospace() << msg;
    if ( isTextStream && ( threshold >= LG_CRIT ) )
    {
      *textStream << getDateString() << CRIT_STR << msg << endl;
    }
  }

  void Logger::crit( const char *msg )
  {
    qCritical().noquote().nospace() << msg;
    if ( isTextStream && ( threshold >= LG_CRIT ) )
    {
      *textStream << getDateString() << CRIT_STR << msg << endl;
    }
  }

  void Logger::crit( const std::string &msg )
  {
    qCritical().noquote().nospace() << msg.c_str();
    if ( isTextStream && ( threshold >= LG_CRIT ) )
    {
      *textStream << getDateString() << CRIT_STR << msg.c_str() << endl;
    }
  }

  void Logger::shutdown()
  {
    if ( isTextStream )
    {
      *textStream << getDateString() << "STOP LOGGING..." << endl;
      textStream->flush();
    }
    if ( logFile->exists() )
    {
      logFile->flush();
      logFile->close();
    }
    isTextStream = false;
  }

  QString Logger::getDateString()
  {
    dateTime = QDateTime::currentDateTime();
    return ( dateTime.toString( dateTimeFormat ) );
  }
}  // namespace bose_soundtoch_lib
