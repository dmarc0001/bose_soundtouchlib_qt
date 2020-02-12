﻿#include "logger.hpp"

#include <QDateTime>
#include <QFileInfo>

namespace bose_commserver
{
  const QString Logger::dateTimeFormat{"[yyyy-MM-dd hh:mm:ss.z] "};
  const QString Logger::NONE_STR{"NONE "};
  const QString Logger::DEBUG_STR{"DEBUG "};
  const QString Logger::INFO_STR{"INFO  "};
  const QString Logger::WARN_STR{"WARN  "};
  const QString Logger::CRIT_STR{"CRIT  "};

  /**
   * @brief Logger::Logger Konstruktor mit Konfigurationsdatei Übergabe
   * @param lFile
   */
  Logger::Logger() : threshold( LG_DEBUG ), logFile( nullptr )
  {
  }

  /**
   * @brief Logger::~Logger
   */
  Logger::~Logger()
  {
    qDebug().noquote().nospace() << "SHUTDOWN LOGGING...";
    // shutdown();
  }

  /**
   * @brief Logger::startLogging
   * @param th
   * @param fileName
   * @return
   */
  int Logger::startLogging( LgThreshold th, const QString &fileName )
  {
    threshold = th;
    //
    // gibt es einen Lognamen
    //
    if ( fileName != nullptr )
    {
      if ( fileName.length() > 4 )
      {
        // Super, das Logfile ist benannt!
        qDebug().noquote().nospace() << "START LOGGING...(" << fileName << ")";
        logFile = std::unique_ptr< QFile >( new QFile( fileName ) );
        logFile->open( QIODevice::WriteOnly | QIODevice::Append );
        textStream = std::unique_ptr< QTextStream >( new QTextStream( logFile.get() ) );
        *textStream << getDateString() << "START LOGGING" << endl;
        if ( logFile && logFile->isOpen() && textStream )
        {
          qDebug().noquote().nospace() << "START LOGGING...OK";
          return ( 1 );
        }
      }
    }
    //
    // Oh, da lief was falsch, LOGGEN nicht möglich
    //
    qDebug().noquote().nospace() << "START LOGGING...FAILED";
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
  LgThreshold Logger::getThreshold()
  {
    return ( threshold );
  }

  /**
   * @brief Logger::warn Ausgabe(n) für WARNUNG
   * @param msg
   */
  void Logger::warn( const QString &msg )
  {
    //! Serialisieren...
    QMutexLocker locker( &logMutex );
    qWarning().noquote().nospace() << msg;
    if ( textStream && threshold >= LG_WARN )
    {
      *textStream << getDateString() << WARN_STR << msg << endl;
    }
  }

  /**
   * @brief Logger::warn
   * @param msg
   */
  void Logger::warn( const char *msg )
  {
    //! Serialisieren...
    QMutexLocker locker( &logMutex );
    qWarning().noquote().nospace() << msg;
    if ( textStream && threshold >= LG_WARN )
    {
      *textStream << getDateString() << WARN_STR << msg << endl;
    }
  }

  /**
   * @brief Logger::warn
   * @param msg
   */
  void Logger::warn( const std::string &msg )
  {
    //! Serialisieren...
    QMutexLocker locker( &logMutex );
    qWarning().noquote().nospace() << msg.c_str();
    if ( textStream && threshold >= LG_WARN )
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
    //! Serialisieren...
    QMutexLocker locker( &logMutex );
    qInfo().noquote().nospace() << msg;
    if ( textStream && threshold >= LG_INFO )
    {
      *textStream << getDateString() << INFO_STR << msg << endl;
    }
  }

  /**
   * @brief Logger::info
   * @param msg
   */
  void Logger::info( const char *msg )
  {
    //! Serialisieren...
    QMutexLocker locker( &logMutex );
    qInfo().noquote().nospace() << msg;
    if ( textStream && threshold >= LG_INFO )
    {
      *textStream << getDateString() << INFO_STR << msg << endl;
    }
  }

  /**
   * @brief Logger::info
   * @param msg
   */
  void Logger::info( const std::string &msg )
  {
    //! Serialisieren...
    QMutexLocker locker( &logMutex );
    qInfo().noquote().nospace() << msg.c_str();
    if ( textStream && threshold >= LG_INFO )
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
    //! Serialisieren...
    QMutexLocker locker( &logMutex );
    qDebug().noquote().nospace() << msg;
    if ( textStream && threshold >= LG_DEBUG )
    {
      *textStream << getDateString() << DEBUG_STR << msg << endl;
    }
  }

  /**
   * @brief Logger::debug
   * @param msg
   */
  void Logger::debug( const char *msg )
  {
    //! Serialisieren...
    QMutexLocker locker( &logMutex );
    qDebug().noquote().nospace() << msg;
    if ( textStream && threshold >= LG_DEBUG )
    {
      *textStream << getDateString() << DEBUG_STR << msg << endl;
    }
  }

  /**
   * @brief Logger::debug
   * @param msg
   */
  void Logger::debug( const std::string &msg )
  {
    //! Serialisieren...
    QMutexLocker locker( &logMutex );
    qDebug().noquote().nospace() << msg.c_str();
    if ( textStream && threshold >= LG_DEBUG )
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
    //! Serialisieren...
    QMutexLocker locker( &logMutex );
    qCritical().noquote().nospace() << msg;
    if ( textStream && threshold >= LG_DEBUG )
    {
      *textStream << getDateString() << CRIT_STR << msg << endl;
    }
  }

  /**
   * @brief Logger::crit
   * @param msg
   */
  void Logger::crit( const char *msg )
  {
    //! Serialisieren...
    QMutexLocker locker( &logMutex );
    qCritical().noquote().nospace() << msg;
    if ( textStream && threshold >= LG_DEBUG )
    {
      *textStream << getDateString() << CRIT_STR << msg << endl;
    }
  }

  /**
   * @brief Logger::crit
   * @param msg
   */
  void Logger::crit( const std::string &msg )
  {
    //! Serialisieren...
    QMutexLocker locker( &logMutex );
    qCritical().noquote().nospace() << msg.c_str();
    if ( textStream && threshold >= LG_DEBUG )
    {
      *textStream << getDateString() << CRIT_STR << msg.c_str() << endl;
    }
  }

  /**
   * @brief Logger::shutdown
   */
  void Logger::shutdown()
  {
    if ( textStream != nullptr )
    {
      textStream->flush();
      // delete(textStream);
      // textStream = nullptr;
    }
    if ( logFile != nullptr )
    {
      logFile->flush();
      logFile->close();
      // delete( logFile );
      // logFile = nullptr;
    }
  }

  /**
   * @brief Logger::getDateString
   * @return
   */
  QString Logger::getDateString()
  {
    dateTime = QDateTime::currentDateTime();
    return ( dateTime.toString( dateTimeFormat ) );
  }

  /**
   * @brief Logger::getThresholdString
   * @param th
   * @return
   */
  const QString Logger::getThresholdString( LgThreshold th )
  {
    switch ( th )
    {
      case LG_NONE:
        return Logger::NONE_STR;
      case LG_CRIT:
        return Logger::CRIT_STR.trimmed();
      case LG_WARN:
        return Logger::WARN_STR.trimmed();
      case LG_INFO:
        return Logger::INFO_STR.trimmed();
      case LG_DEBUG:
        return Logger::DEBUG_STR.trimmed();
    }
    return QString( "unknown" );
  }
}  // namespace bose_commserver
