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

  Logger &operator<<( Logger &lg, const LgThreshold th )
  {
    lg.setCurrentThreshold( th );
    return lg;
  }

  Logger &operator<<( Logger &lg, const LgMod mod )
  {
    qDebug() << "################ operator for LgMod #############################";
    lg.debug( "################ operator for LgMod #############################" );
    switch ( mod )
    {
      case LgMod::endl:
        lg.lineEnd();
        break;
      case LgMod::currDate:
        lg.printCurrDateStr();
    }
    return lg;
  }

  //#######################################################################################
  //#### der Logger                                                                    ####
  //#######################################################################################

  /**
   * @brief Logger::Logger Konstruktor mit Konfigurationsdatei Übergabe
   * @param lFile
   */
  Logger::Logger() : threshold( LG_DEBUG ), currentThreshold( LG_DEBUG ), logFile( nullptr )
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

  void Logger::lineEnd()
  {
    // if ( textStream && threshold >= LG_CRIT )
    {
      *textStream << ::Qt::endl;
      textStream->flush();
    }
  }

  void Logger::printCurrDateStr()
  {
    // if ( textStream && threshold >= LG_CRIT )
    {
      *textStream << getDateString();
    }
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

  /**
   * @brief Logger::getThresholdFromString
   * @param th
   * @return
   */
  LgThreshold Logger::getThresholdFromString( const QString &th )
  {
    if ( th.toUpper().compare( Logger::NONE_STR.trimmed() ) == 0 )
      return ( LG_NONE );
    else if ( th.toUpper().compare( Logger::CRIT_STR.trimmed() ) == 0 )
      return ( LG_CRIT );
    else if ( th.toUpper().compare( Logger::WARN_STR.trimmed() ) == 0 )
      return ( LG_WARN );
    else if ( th.toUpper().compare( Logger::INFO_STR.trimmed() ) == 0 )
      return ( LG_INFO );
    else if ( th.toUpper().compare( Logger::DEBUG_STR.trimmed() ) == 0 )
      return ( LG_DEBUG );
    return ( LG_DEBUG );
  }

}  // namespace bose_commserver
