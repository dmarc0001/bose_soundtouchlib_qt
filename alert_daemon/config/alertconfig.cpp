#include "alertconfig.hpp"

#include <QDebug>

namespace bose_commserver
{
  AlertAppConfig::AlertAppConfig()
      : logFileName( AlertAppConfig::defaultLogFile )
      , bindaddr( AlertAppConfig::defaultBindAddr )
      , bindport( AlertAppConfig::defaultBindPort )
      , wsPort( AlertAppConfig::defaultWsport )
      , httpPort( AlertAppConfig::defaultHttpPort )
      , logPath( AlertAppConfig::defaultLogPath )
      , threshold( AlertAppConfig::defaultThreshold )
      , configFileName( QString( "./%1.ini" ).arg( QCoreApplication::applicationName() ) )
      , isDebug( false )
      , isDebugManual( false )
      , isLogfileManual( false )
      , isBindAddrManual( false )
      , isBindPortManual( false )
      , haveToCreateConfigFile( false )
      , isHashValid( false )
      , lg( nullptr )
      , alConfigs( std::shared_ptr< AlertList >( new AlertList() ) )
  {
    availDevices.clear();
    qDebug().noquote() << "AlertAppConfig::AlertAppConfig()";
  }

  AlertAppConfig::~AlertAppConfig()
  {
    qDebug().noquote() << "AlertAppConfig::~AlertAppConfig()";
    saveSettings();
  }

  //###########################################################################
  //###########################################################################
  //#### HASHING                                                           ####
  //###########################################################################
  //###########################################################################
  QByteArray AlertAppConfig::getConfigHash()
  {
    if ( isHashValid )
      return configHash;
    //
    // Hash errechnen
    //
    QCryptographicHash allHashObj( QCryptographicHash::Md5 );

    allHashObj.addData( logFileName.toUtf8() );
    allHashObj.addData( bindaddr.toUtf8() );
    allHashObj.addData( bindport.toUtf8() );
    allHashObj.addData( wsPort.toUtf8() );
    allHashObj.addData( httpPort.toUtf8() );
    allHashObj.addData( logPath.toUtf8() );
    allHashObj.addData( QString( "%1" ).arg( static_cast< int >( threshold ) ).toUtf8() );
    allHashObj.addData( configFileName.toUtf8() );
    // allHashObj.addData( availDevices.join( ',' ).toUtf8() );
    allHashObj.addData( QString( "%1" ).arg( isDebug ).toUtf8() );
    allHashObj.addData( QString( "%1" ).arg( isDebugManual ).toUtf8() );
    allHashObj.addData( QString( "%1" ).arg( isLogfileManual ).toUtf8() );
    allHashObj.addData( QString( "%1" ).arg( isBindAddrManual ).toUtf8() );
    allHashObj.addData( QString( "%1" ).arg( isBindPortManual ).toUtf8() );
    //
    // alle alertconfigs dazu
    //
    for ( SingleAlertConfig ac : *( alConfigs.get() ) )
    {
      allHashObj.addData( ac.getConfigHash() );
    }
    configHash = allHashObj.result().toHex().toUpper();
    return configHash;
  }

  //###########################################################################
  //###########################################################################
  //#### LOAD                                                              ####
  //###########################################################################
  //###########################################################################

  /**
   * @brief AlertAppConfig::loadSettings
   * Lade Einstellungen aus der Datei
   * @return
   */
  bool AlertAppConfig::loadSettings()
  {
    qDebug().noquote() << "AlertAppConfig::loadSettings(), logfile: " << getFullLogFilePath();
    isHashValid = false;
    //
    // Config leeren
    //
    alConfigs->clear();
    //
    // Ini Dateihandling initialisieren, Datei einlesen
    //

    // TODO: feststellen ob es die datei gibt
    /*
    qInfo().noquote() << "AlertAppConfig::AlertAppConfig(): not found config file " << configFileName << " have to create it.";
    haveToCreateConfigFile = true;
    */

    QSettings settings( configFileName, QSettings::IniFormat );
    //
    // die verschiedenen Sektionen behandeln
    //
    if ( !loadLogSettings( settings ) )
    {
      makeDefaultLogSettings( settings );
    }
    if ( !loadAppSettings( settings ) )
    {
      makeAppDefaultSettings( settings );
    }
    //
    // suche alle möglichen alarme
    //
    QString alertGroupBodySearch( QString( "^%1" ).arg( AlertAppConfig::constAlertGroupPattern ) );
    //
    // alle Gruppen auslesen und nach alert filtern
    //
    QStringList groups = settings.childGroups().filter( QRegularExpression( alertGroupBodySearch ) );
    //
    // und nun alle durch lesen
    //
    for ( QString currentAlertGroup : groups )
    {
      //
      // lese Einstellungen
      //
      loadAlertSetting( settings, currentAlertGroup );
    }
    //
    // wenn es noch keine Konfig datei gab, schreibe eine
    //
    if ( haveToCreateConfigFile )
    {
      saveSettings();
    }
    return ( true );
  }

  /**
   * @brief AlertAppConfig::loadSettings
   * Lade Einstellungen aus einer angegebenen Datei
   * @param configFile
   * @return
   */
  bool AlertAppConfig::loadSettings( const QString &configFile )
  {
    qDebug().noquote() << "AlertAppConfig::loadSettings(" << configFile << ")...";
    configFileName = configFile;
    return loadSettings();
  }

  /**
   * @brief AlertAppConfig::loadLogSettings
   * @param settings
   * @return
   */
  bool AlertAppConfig::loadLogSettings( QSettings &settings )
  {
    bool retval = true;
    qDebug().noquote() << "AlertAppConfig::loadLogSettings() START...";
    //
    // Öffne die Gruppe Logeinstellungen
    //
    settings.beginGroup( AlertAppConfig::constLogGroupName );
    //
    // lese den Logpfad aus
    //
    logPath = settings.value( AlertAppConfig::constKeyLogPath, AlertAppConfig::constNoData ).toString();
    if ( QString::compare( logPath, AlertAppConfig::constNoData ) == 0 )
    {
      // Nicht gefunden -> Fehler markieren
      retval = false;
    }
    qDebug().noquote().nospace() << "AlertAppConfig::loadLogSettings(): Logfile Path: <" << logPath << ">";
    //
    // Lese den Dateinamen aus
    //
    logFileName = settings.value( AlertAppConfig::constKeyLogFile, AlertAppConfig::constNoData ).toString();
    if ( QString::compare( logFileName, AlertAppConfig::constNoData ) == 0 )
    {
      // Nicht gefunden -> Fehler markieren
      retval = false;
    }
    qDebug().noquote().nospace() << "AlertAppConfig::loadLogSettings(): Logfile: <" << logFileName << ">";
    //
    // lese loglevel aus
    //
    threshold = static_cast< LgThreshold >(
        settings.value( AlertAppConfig::constLogThresholdKey, static_cast< int >( LgThreshold::LG_NONE ) ).toInt() & 0xff );
    if ( threshold == LgThreshold::LG_NONE )
    {
      threshold = AlertAppConfig::defaultThreshold;
    }
    qDebug().noquote().nospace() << "AlertAppConfig::loadLogSettings(): loglevel: <" << Logger::getThresholdString( threshold ) << ">";
    //
    // Ende der Gruppe
    //
    settings.endGroup();
    //
    // Ergebnis kommunizieren
    //
    qDebug().noquote() << "AlertAppConfig::loadLogSettings() END...";
    return retval;
  }

  /**
   * @brief AlertAppConfig::makeDefaultLogSettings
   * @param settings
   */
  void AlertAppConfig::makeDefaultLogSettings( QSettings &settings )
  {
    qDebug().noquote() << "AlertAppConfig::makeDefaultLogSettings()";
    //
    // Öffne die Gruppe Logeinstellungen
    //
    settings.beginGroup( AlertAppConfig::constLogGroupName );
    //
    // defaultwerte setzten
    //
    logPath = QStandardPaths::writableLocation( QStandardPaths::AppDataLocation );
    settings.setValue( AlertAppConfig::constKeyLogPath, logPath );
    logFileName = QCoreApplication::applicationName().append( ".log" );
    settings.setValue( AlertAppConfig::constKeyLogFile, logFileName );
    threshold = AlertAppConfig::defaultThreshold;
    settings.setValue( AlertAppConfig::constLogThresholdKey, static_cast< int >( threshold ) );
    //
    // Ende der Gruppe
    //
    settings.endGroup();
  }

  /**
   * @brief AlertAppConfig::loadAlertSetting
   * @param currentAlertGroup
   * @return
   */
  bool AlertAppConfig::loadAlertSetting( QSettings &settings, const QString &currentAlertGroup )
  {
    qDebug().noquote() << "AlertAppConfig::loadAlertSetting(" << currentAlertGroup << ")...";
    //
    // erst mal ein Objekt erstellen
    //
    SingleAlertConfig currConfig( currentAlertGroup );
    //
    // in die Gruppe wechseln
    //
    settings.beginGroup( currentAlertGroup );
    //
    // lese avisierte Lautstärke
    //
    currConfig.setAlVolume( static_cast< qint8 >( settings.value( AlertAppConfig::constAlertVolumeKey, 1 ).toInt() & 0xff ) );
    qDebug().noquote().nospace() << "AlertAppConfig::loadAlertSetting(): alert volume: <" << currConfig.getAlVolume() << ">";
    //
    // lese ob langsam ansteigende Lautstärke gewünscht ist
    //
    currConfig.setAlRaiseVolume( settings.value( AlertAppConfig::constAlertRaiseVolKey, false ).toBool() );
    qDebug().noquote().nospace() << "AlertAppConfig::loadAlertSetting(): alert volume raising: <" << currConfig.getAlRaiseVolume()
                                 << ">";
    //
    // lese wiel lange die Lautstärke ansteigen soll
    //
    currConfig.setAlDuration(
        static_cast< qint16 >( settings.value( AlertAppConfig::constAlertDurationKey, 1000 ).toInt() & 0xffff ) );
    qDebug().noquote().nospace() << "AlertAppConfig::loadAlertSetting(): alert volume raising duration time: <"
                                 << currConfig.getAlDuration() << ">";
    //
    // lese das Datum, wenn vorhanden
    // ansonsten leeres Datum
    //
    currConfig.setAlDate( settings.value( AlertAppConfig::constAlertDateKey, QDate() ).toDate() );
    if ( currConfig.getAlDate().isValid() )
      qDebug().noquote().nospace() << "AlertAppConfig::loadAlertSetting(): alert date: <" << currConfig.getAlDate() << ">";
    //
    // lese alarmzeit
    // hier baue ich selber hh:mm (24 h Format)
    //
    QString timeStr = settings.value( AlertAppConfig::constAlertTimeKey, "06:00" ).toString().trimmed();
    // Konmtrolle ob der Stimg sinnvoll ist
    QRegularExpression re( AlertAppConfig::constPatternTime );
    QRegularExpressionMatch match = re.match( timeStr );
    // und testen
    if ( match.hasMatch() )
    {
      //
      // gültiges Zeitformat, splitten nach Stunde, Minute
      //
      QStringList hhmm = timeStr.split( ":" );
      currConfig.setAlTime( QTime( hhmm[ 0 ].toInt(), hhmm[ 1 ].toInt() ) );
      // qDebug().noquote().nospace() << "AlertAppConfig::loadAlertSetting(): alert time: <" << hhmm[ 0 ] << ":" << hhmm[ 1 ] << ">";
      qDebug().noquote().nospace() << "AlertAppConfig::loadAlertSetting(): alert time: <" << currConfig.getAlTime().toString( "HH:mm" )
                                   << ">";
    }
    else
    {
      //
      // default 0600
      //
      currConfig.setAlTime( QTime( 6, 0 ) );
      qDebug().noquote().nospace() << "AlertAppConfig::loadAlertSetting(): alert time: <06:00> (default)";
    }
    //
    // an welchen Tagen?
    // erwartet eine Liste mit wochentagen, getrennt mit Komma
    // mo,tu,we,th,fr,sa,su
    // konvertiert nach 1,2,3,4,5,6,7
    //
    QStringList days = settings.value( AlertAppConfig::constAlertDaysKey, "all" ).toStringList();
    if ( QString::compare( days[ 0 ].toLower(), "all" ) == 0 || days.isEmpty() )
    {
      //
      // jeden Tag
      //
      QList< qint8 > lst;
      lst << 1 << 2 << 3 << 4 << 5 << 6 << 7;
      //
      // in die Config
      //
      currConfig.setAlDays( lst );
      qDebug().noquote().nospace() << "AlertAppConfig::loadAlertSetting(): alert days: all";
    }
    else
    {
      QList< qint8 > lst;
      for ( QString day : days )
      {
        if ( QString::compare( day.trimmed().toLower(), "mo" ) == 0 )
          lst << 1;
        else if ( QString::compare( day.trimmed().toLower(), "tu" ) == 0 )
          lst << 2;
        else if ( QString::compare( day.trimmed().toLower(), "we" ) == 0 )
          lst << 3;
        else if ( QString::compare( day.trimmed().toLower(), "th" ) == 0 )
          lst << 4;
        else if ( QString::compare( day.trimmed().toLower(), "fr" ) == 0 )
          lst << 5;
        else if ( QString::compare( day.trimmed().toLower(), "sa" ) == 0 )
          lst << 6;
        else if ( QString::compare( day.trimmed().toLower(), "su" ) == 0 )
          lst << 7;
      }
      //
      // in die Config
      //
      currConfig.setAlDays( lst );
      qDebug().noquote().nospace() << "AlertAppConfig::loadAlertSetting(): alert days: <" << currConfig.getAlDays() << ">";
    }
    //
    // source / programm
    //
    currConfig.setAlSource( settings.value( AlertAppConfig::constAlertSourceKey, "PRESET_1" ).toString() );
    qDebug().noquote().nospace() << "AlertAppConfig::loadAlertSetting(): alert source: <" << currConfig.getAlSource() << ">";
    //
    // source account
    //
    currConfig.setAlSourceAccount( settings.value( AlertAppConfig::constAlertAccountKey, "" ).toString() );
    qDebug().noquote().nospace() << "AlertAppConfig::loadAlertSetting(): alert source account: <" << currConfig.getAlSourceAccount()
                                 << ">";
    //
    // type der source
    //
    currConfig.setAlType( settings.value( AlertAppConfig::constAlertTypeKey, "" ).toString() );
    qDebug().noquote().nospace() << "AlertAppConfig::loadAlertSetting(): alert source type: <" << currConfig.getAlType() << ">";
    //
    // Alarm erlaubt?
    //
    currConfig.setAlEnable( settings.value( AlertAppConfig::constAlertEnableKey, false ).toBool() );
    //
    // auf welchen Geräten?
    //
    QStringList devs = settings.value( AlertAppConfig::constAlertDevicesKey ).toStringList();
    if ( devs.isEmpty() )
    {
      //
      // leere Liste, kein Gerät
      // ==> kein Alarm
      //
      currConfig.setAlDevices( QStringList() );
      currConfig.setAlEnable( false );
    }
    else
    {
      // in die Config
      currConfig.setAlDevices( devs );
    }
    qDebug().noquote().nospace() << "AlertAppConfig::loadAlertSetting(): alert on devices: <" << currConfig.getAlDevices() << ">";
    qDebug().noquote().nospace() << "AlertAppConfig::loadAlertSetting(): alert enable: <" << currConfig.getAlEnable() << ">";
    //
    // Bemerkungen
    //
    currConfig.setAlNote( settings.value( AlertAppConfig::constAlertNoteKey, "" ).toString() );
    if ( currConfig.getAlNote().size() > 1 )
      qDebug().noquote().nospace() << "AlertAppConfig::loadAlertSetting(): alert note: <" << currConfig.getAlNote() << ">";
    //
    // location für soundtouch
    //
    currConfig.setAlLocation( settings.value( AlertAppConfig::constAlertLocationKey, "" ).toString() );
    qDebug().noquote().nospace() << "AlertAppConfig::loadAlertSetting(): alert location: <" << currConfig.getAlLocation() << ">";
    //
    settings.endGroup();
    alConfigs->append( currConfig );
    return true;
  }

  /**
   * @brief AlertAppConfig::loadAppSettings
   * @param settings
   * @return
   */
  bool AlertAppConfig::loadAppSettings( QSettings &settings )
  {
    bool retval = true;
    qDebug().noquote() << "AlertAppConfig::loadAppSettings() START...";
    //
    // Öffne die Gruppe app
    //
    settings.beginGroup( AlertAppConfig::constAppGroupName );
    //
    // Bindadresse des Dienstes / der app
    //
    bindaddr = settings.value( AlertAppConfig::constBindAddrKey, AlertAppConfig::defaultBindAddr ).toString();
    qDebug().noquote() << "AlertAppConfig::loadAppSettings(): bindaddr for app: " << bindaddr;
    //
    // Bindport für die App
    //
    bindport = settings.value( AlertAppConfig::constBindPortKey, AlertAppConfig::defaultBindPort ).toString();
    qDebug().noquote() << "AlertAppConfig::loadAppSettings(): bindport for app: " << bindport;
    //
    // default port for ws connect to devices
    //
    wsPort = settings.value( AlertAppConfig::constWsportKey, AlertAppConfig::defaultWsport ).toString();
    qDebug().noquote() << "AlertAppConfig::loadAppSettings(): dest ws port for devices: " << wsPort;
    //
    // default port for http connect to devices
    //
    httpPort = settings.value( AlertAppConfig::constHttpPortKey, AlertAppConfig::defaultHttpPort ).toString();
    qDebug().noquote() << "AlertAppConfig::loadAppSettings(): dest http port for devices: " << httpPort;
    //
    // Ende der Gruppe
    //
    settings.endGroup();
    //
    // Ergebnis kommunizieren
    //
    return retval;
  }

  void AlertAppConfig::makeAppDefaultSettings( QSettings &settings )
  {
    qDebug().noquote() << "AlertAppConfig::loadAppSettings() START...";
    //
    // Öffne die Gruppe app
    //
    settings.beginGroup( AlertAppConfig::constAppGroupName );
    //
    // default werte setzen
    //
    bindaddr = AlertAppConfig::defaultBindAddr;
    settings.setValue( AlertAppConfig::constBindAddrKey, bindaddr );
    bindport = AlertAppConfig::defaultBindPort;
    settings.setValue( AlertAppConfig::constBindPortKey, bindport );
    wsPort = AlertAppConfig::defaultWsport;
    settings.setValue( AlertAppConfig::constWsportKey, wsPort );
    httpPort = AlertAppConfig::defaultHttpPort;
    settings.setValue( AlertAppConfig::constHttpPortKey, httpPort );
    //
    // Gruppe schliessen
    //
    settings.endGroup();
  }

  //###########################################################################
  //###########################################################################
  //#### SAVE                                                              ####
  //###########################################################################
  //###########################################################################

  bool AlertAppConfig::saveSettings( void )
  {
    qDebug().noquote() << "AlertAppConfig::saveSettings()...";
    //
    // Ini Dateihandling initialisieren, Datei vorbereiten
    // zunächst DAtei kopieren
    //
    QString duplicatFileName = duplicateConfigFile( configFileName );
    //
    // config vorbereiten
    //
    QSettings settings( configFileName, QSettings::IniFormat );
    settings.clear();
    //
    // die verschiedenen Sektionen behandeln
    //
    if ( !saveLogSettings( settings ) )
    {
      resetConfigFile( duplicatFileName );
      return false;
    }
    if ( !saveAppSettings( settings ) )
    {
      resetConfigFile( duplicatFileName );
      return false;
    }
    for ( SingleAlertConfig ac : *( alConfigs.get() ) )
    {
      if ( !saveAlertSettings( settings, ac ) )
      {
        resetConfigFile( duplicatFileName );
        return false;
      }
    }
    //
    // eigentlich nicht nötig
    //
    settings.sync();
    haveToCreateConfigFile = false;
    return true;
  }

  /**
   * @brief AlertAppConfig::duplicateConfigFile
   * @param fileName
   * @return
   */
  QString AlertAppConfig::duplicateConfigFile( const QString &fileName )
  {
    qDebug().noquote() << "AlertAppConfig::duplicateConfigFile()...";
    //
    // Ini Dateihandling initialisieren, Datei vorbereiten
    // zunächst DAtei kopieren
    //
    QDateTime now( QDateTime::currentDateTime() );
    QString newFileName = QString( "%1-%2" ).arg( now.toString( "yyyyMMddHHmmss" ) ).arg( fileName );
    QFile cFile( fileName );
    cFile.copy( newFileName );
    return newFileName;
  }

  /**
   * @brief AlertAppConfig::resetConfigFile
   * @param duplicatFileName
   * @return
   */
  bool AlertAppConfig::resetConfigFile( const QString &duplicatFileName )
  {
    qDebug().noquote() << "AlertAppConfig::resetConfigFile()...";
    //
    // Ini Dateihandling initialisieren, Datei vorbereiten
    // zunächst DAtei kopieren
    //
    QFile::remove( configFileName );
    QFile cFile( duplicatFileName );
    return cFile.copy( configFileName );
  }

  /**
   * @brief AlertAppConfig::saveLogSettings
   * @param settings
   * @return
   */
  bool AlertAppConfig::saveLogSettings( QSettings &settings )
  {
    qDebug().noquote() << "AlertAppConfig::saveLogSettings() START...";
    //
    // Öffne die Gruppe Logeinstellungen
    //
    settings.beginGroup( AlertAppConfig::constLogGroupName );
    settings.setValue( AlertAppConfig::constKeyLogPath, logPath );
    settings.setValue( AlertAppConfig::constKeyLogFile, logFileName );
    settings.setValue( AlertAppConfig::constLogThresholdKey, static_cast< int >( threshold ) );
    //
    // Ende der Gruppe
    //
    settings.endGroup();
    //
    // Ergebnis kommunizieren
    //
    qDebug().noquote() << "AlertAppConfig::saveLogSettings() END...";
    return true;
  }

  /**
   * @brief AlertAppConfig::saveAppSettings
   * @param settings
   * @return
   */
  bool AlertAppConfig::saveAppSettings( QSettings &settings )
  {
    qDebug().noquote() << "AlertAppConfig::saveAppSettings() START...";
    //
    // Öffne die Gruppe app
    //
    settings.beginGroup( AlertAppConfig::constAppGroupName );
    settings.setValue( AlertAppConfig::constBindAddrKey, bindaddr );
    settings.setValue( AlertAppConfig::constBindPortKey, bindport );
    settings.setValue( AlertAppConfig::constWsportKey, wsPort );
    settings.setValue( AlertAppConfig::constHttpPortKey, httpPort );
    //
    // Ende der Gruppe
    //
    settings.endGroup();
    qDebug().noquote() << "AlertAppConfig::saveAppSettings() END...";
    return true;
  }

  /**
   * @brief AlertAppConfig::saveAlertConfig
   * @param settings
   * @param SingleAlertConfig
   * @return
   */
  bool AlertAppConfig::saveAlertSettings( QSettings &settings, const SingleAlertConfig &currConfig )
  {
    qDebug().noquote() << "AlertAppConfig::saveAlertSettings(" << currConfig.getName() << ")...";
    //
    // in die Gruppe wechseln
    //
    settings.beginGroup( currConfig.getName() );
    settings.setValue( AlertAppConfig::constAlertVolumeKey, currConfig.getAlVolume() );
    settings.setValue( AlertAppConfig::constAlertRaiseVolKey, currConfig.getAlRaiseVolume() );
    settings.setValue( AlertAppConfig::constAlertDurationKey, currConfig.getAlDuration() );
    settings.setValue( AlertAppConfig::constAlertDateKey, currConfig.getAlDate().toString( "yyyy-MM-dd" ) );
    settings.setValue( AlertAppConfig::constAlertTimeKey, currConfig.getAlTime().toString( "HH:mm" ) );
    QList< qint8 > dlist( currConfig.getAlDays() );
    QStringList days;
    for ( qint8 daynum : dlist )
    {
      switch ( daynum )
      {
        case 1:
          days << "mo";
          break;
        case 2:
          days << "tu";
          break;
        case 3:
          days << "we";
          break;
        case 4:
          days << "th";
          break;
        case 5:
          days << "fr";
          break;
        case 6:
          days << "sa";
          break;
        case 7:
        default:
          days << "su";
          break;
      }
    }
    settings.setValue( AlertAppConfig::constAlertDaysKey, days );
    settings.setValue( AlertAppConfig::constAlertSourceKey, currConfig.getAlSource() );
    settings.setValue( AlertAppConfig::constAlertAccountKey, currConfig.getAlSourceAccount() );
    settings.setValue( AlertAppConfig::constAlertTypeKey, currConfig.getAlType() );
    settings.setValue( AlertAppConfig::constAlertEnableKey, currConfig.getAlEnable() );
    settings.setValue( AlertAppConfig::constAlertDevicesKey, currConfig.getAlDevices() );
    settings.setValue( AlertAppConfig::constAlertNoteKey, currConfig.getAlNote() );
    settings.setValue( AlertAppConfig::constAlertLocationKey, currConfig.getAlLocation() );
    //
    settings.endGroup();
    return true;
  }

  //###########################################################################
  //###########################################################################
  //#### SET/READ                                                          ####
  //###########################################################################
  //###########################################################################

  QStringList AlertAppConfig::getAvailDevices() const
  {
    return availDevices;
  }

  void AlertAppConfig::setAvailDevices( const QStringList &value )
  {
    availDevices = value;
  }

  void AlertAppConfig::addAvailDevices( const QString &dev )
  {
    availDevices.append( dev );
  }

  QRegularExpression AlertAppConfig::getAlertPattern()
  {
    QRegularExpression re( AlertAppConfig::constAlertGroupPattern );
    return re;
  }

  QString AlertAppConfig::getDefaultWsPort() const
  {
    return wsPort;
  }

  QString AlertAppConfig::getDefaultHttpPort() const
  {
    return httpPort;
  }

  QString AlertAppConfig::getDefaultLogFile()
  {
    return QString( AlertAppConfig::defaultLogFile );
  }

  QString AlertAppConfig::getDefaultBindAddr()
  {
    return QString( AlertAppConfig::defaultBindAddr );
  }

  QString AlertAppConfig::getDefaultBindPort()
  {
    return QString( AlertAppConfig::defaultBindPort );
  }

  QString AlertAppConfig::getLogFileName() const
  {
    return logFileName;
  }

  void AlertAppConfig::setLogFileName( const QString &value )
  {
    logFileName = value;
    isHashValid = false;
    isLogfileManual = true;
  }

  QString AlertAppConfig::getBindaddr() const
  {
    return bindaddr;
  }

  void AlertAppConfig::setBindaddr( const QString &value )
  {
    bindaddr = value;
    isHashValid = false;
    isBindAddrManual = true;
  }

  QString AlertAppConfig::getBindport() const
  {
    return bindport;
  }

  void AlertAppConfig::setBindport( const QString &value )
  {
    bindport = value;
    isHashValid = false;
    isBindPortManual = true;
  }

  LgThreshold AlertAppConfig::getThreshold() const
  {
    return threshold;
  }

  void AlertAppConfig::setThreshold( const LgThreshold &value )
  {
    threshold = value;
    isHashValid = false;
  }

  std::shared_ptr< Logger > AlertAppConfig::getLogger() const
  {
    return lg;
  }

  void AlertAppConfig::setLogger( const std::shared_ptr< Logger > &value )
  {
    lg = value;
    isHashValid = false;
  }

  std::shared_ptr< AlertList > AlertAppConfig::getAlConfigs() const
  {
    return alConfigs;
  }

  bool AlertAppConfig::getIsDebug() const
  {
    return isDebug;
  }

  void AlertAppConfig::setIsDebug( bool value )
  {
    isDebug = value;
    isHashValid = false;
    isDebugManual = true;
  }

  QString AlertAppConfig::getConfigFile() const
  {
    return ( configFileName );
  }

  QString AlertAppConfig::getLogfileName( void ) const
  {
    return logFileName;
  }

  QString AlertAppConfig::getLogfilePath( void ) const
  {
    return logPath;
  }

  void AlertAppConfig::setLogfileName( const QString &value )
  {
    logFileName = value;
    isHashValid = false;
  }

  void AlertAppConfig::setLogFilePath( const QString &value )
  {
    logPath = value;
    isHashValid = false;
  }

  /**
   * @brief AlertAppConfig::getFullLogFilePath
   * @return
   */
  QString AlertAppConfig::getFullLogFilePath( void )
  {
    // um korrekte Pfade zu erzeugen ein QFileobjekt
    QDir lp( logPath );
    if ( lp.exists() && lp.isReadable() )
    {
      logPath = lp.dirName();
    }
    else
    {
      logPath = ".";
    }
    QFile logFile( QString().append( logPath ).append( "/" ).append( logFileName ) );
    // Fileinfo zur korrekten erzeugung des kompletten Pfdades
    QFileInfo logFileInfo( logFile );
    return ( logFileInfo.filePath() );
  }

  //###########################################################################
  //###########################################################################
  //#### Configclass                                                       ####
  //###########################################################################
  //###########################################################################
  /**
   * @brief SingleAlertConfig::SingleAlertConfig
   * @param _name
   */
  SingleAlertConfig::SingleAlertConfig( const QString &_name )
      : alName( _name )
      , alVolume( 10 )
      , alDate( QDate() )
      , alTime( QTime() )
      , alLocation( "" )
      , alSource( "" )
      , alRaiseVolume( false )
      , alDuration( 0 )
      , alSourceAccount( "" )
      , alType( "" )
      , alEnable( false )
      , alNote( "" )
      , isHashValid( false )
  {
    alDays.clear();
    alDevices.clear();
    alertHash.clear();
  }

  QByteArray SingleAlertConfig::getConfigHash()
  {
    if ( isHashValid )
      return alertHash;
    //
    // Hash errechnen
    //
    QCryptographicHash allHashObj( QCryptographicHash::Md5 );
    allHashObj.addData( alName.toUtf8() );
    allHashObj.addData( QString( alVolume ).toUtf8() );
    if ( alDate.isValid() )
      allHashObj.addData( alDate.toString( "yyyyMMdd" ).toUtf8() );
    allHashObj.addData( alTime.toString( "hhmm" ).toUtf8() );
    allHashObj.addData( alLocation.toUtf8() );
    allHashObj.addData( alSource.toUtf8() );
    allHashObj.addData( QString( "%1" ).arg( alRaiseVolume ).toUtf8() );
    allHashObj.addData( QString( "%1" ).arg( alDuration ).toUtf8() );
    allHashObj.addData( alSourceAccount.toUtf8() );
    allHashObj.addData( alType.toUtf8() );
    allHashObj.addData( QString( "%1" ).arg( alEnable ).toUtf8() );
    allHashObj.addData( alNote.toUtf8() );
    alertHash = allHashObj.result().toHex().toUpper();
    return alertHash;
  }

  QStringList SingleAlertConfig::getAlDaysStr() const
  {
    QStringList days;
    for ( qint8 daynum : alDays )
    {
      switch ( daynum )
      {
        case 1:
          days << "mo";
          break;
        case 2:
          days << "tu";
          break;
        case 3:
          days << "we";
          break;
        case 4:
          days << "th";
          break;
        case 5:
          days << "fr";
          break;
        case 6:
          days << "sa";
          break;
        case 7:
        default:
          days << "su";
          break;
      }
    }
    return days;
  }

  QString SingleAlertConfig::getName() const
  {
    return alName;
  }

  qint8 SingleAlertConfig::getAlVolume() const
  {
    return alVolume;
  }

  void SingleAlertConfig::setAlVolume( const qint8 &value )
  {
    alVolume = value;
    isHashValid = false;
  }

  QDate SingleAlertConfig::getAlDate() const
  {
    return alDate;
  }

  void SingleAlertConfig::setAlDate( const QDate &value )
  {
    alDate = value;
    isHashValid = false;
  }

  QTime SingleAlertConfig::getAlTime() const
  {
    return alTime;
  }

  void SingleAlertConfig::setAlTime( const QTime &value )
  {
    alTime = value;
    isHashValid = false;
  }

  QString SingleAlertConfig::getAlLocation() const
  {
    return alLocation;
  }

  void SingleAlertConfig::setAlLocation( const QString &value )
  {
    alLocation = value;
    isHashValid = false;
  }

  QString SingleAlertConfig::getAlSource() const
  {
    return alSource;
  }

  void SingleAlertConfig::setAlSource( const QString &value )
  {
    alSource = value;
    isHashValid = false;
  }

  bool SingleAlertConfig::getAlRaiseVolume() const
  {
    return alRaiseVolume;
  }

  void SingleAlertConfig::setAlRaiseVolume( bool value )
  {
    alRaiseVolume = value;
    isHashValid = false;
  }

  qint16 SingleAlertConfig::getAlDuration() const
  {
    return alDuration;
  }

  void SingleAlertConfig::setAlDuration( const qint16 &value )
  {
    alDuration = value;
    isHashValid = false;
  }

  QString SingleAlertConfig::getAlSourceAccount() const
  {
    return alSourceAccount;
  }

  void SingleAlertConfig::setAlSourceAccount( const QString &value )
  {
    alSourceAccount = value;
    isHashValid = false;
  }

  QList< qint8 > SingleAlertConfig::getAlDays() const
  {
    return alDays;
  }

  void SingleAlertConfig::setAlDays( const QList< qint8 > &value )
  {
    alDays = value;
    isHashValid = false;
  }

  QStringList SingleAlertConfig::getAlDevices() const
  {
    return alDevices;
  }

  void SingleAlertConfig::setAlDevices( const QStringList &value )
  {
    alDevices = value;
    isHashValid = false;
  }

  QString SingleAlertConfig::getAlType() const
  {
    return alType;
  }

  void SingleAlertConfig::setAlType( const QString &value )
  {
    alType = value;
    isHashValid = false;
  }

  bool SingleAlertConfig::getAlEnable() const
  {
    return alEnable;
  }

  void SingleAlertConfig::setAlEnable( bool value )
  {
    alEnable = value;
    isHashValid = false;
  }

  QString SingleAlertConfig::getAlNote() const
  {
    return alNote;
  }

  void SingleAlertConfig::setAlNote( const QString &value )
  {
    alNote = value;
    isHashValid = false;
  }

}  // namespace bose_commserver
