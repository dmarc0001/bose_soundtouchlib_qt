#include "commandgethandler.hpp"

namespace bose_commserver
{
  /**
   * @brief CommandGetHandler::CommandGetHandler
   * @param dconfig
   * @param request
   */
  CommandGetHandler::CommandGetHandler( AppConfigPtr dconfig, QJsonObject request ) : config( dconfig ), gRequest( request )
  {
    lg = config->getLogger();
    *lg << LINFO << "CommandGetHandler::CommandGetHandler: create object..." << Qt::endl;
  }

  /**
   * @brief CommandGetHandler::getResponse
   * @return
   */
  JSonStringPtr CommandGetHandler::getResponse()
  {
    JSonStringPtr answer;
    //
    // es ist ein get Kommando, wurde vor der Instanzierung geprüft
    //
    *lg << LDEBUG << "CommandGetHandler::getResponse: get answer..." << Qt::endl;
    //
    // ist es ein Objekt und ist es ein GET Objekt?
    //
    if ( !gRequest[ command::getCmd ].isNull() && gRequest[ command::getCmd ].isObject() )
    {
      JsonObjSPtr jsonObj = JsonObjSPtr( new QJsonObject() );
      //
      // gib mal das anforderungsobjekt (da objekt mit den get-Anforderungen)
      //
      *lg << LDEBUG << "CommandGetHandler::getResponse: get request is acknolowedged...";
      //
      // eine map draus machen, die kann ich gut abarbeiten
      //
      QVariantMap getMap = gRequest[ command::getCmd ].toObject().toVariantMap();
      //
      // arbeite die Map ab, jedes Element
      //
      for ( auto it = getMap.begin(); it != getMap.end(); it++ )
      {
        //
        // es können nur Strings da sein
        //
        QString key = it.key();
        QString val = it.value().toString();
        *lg << LDEBUG << "CommandGetHandler::getResponse: get params: <" << key << "> <" << val << ">..." << Qt::endl;
        //
        // Config anfrage?
        //
        if ( key.compare( command::getConfigCmd ) == 0 )
        {
          //
          // Einstellungen des daemon
          //
          if ( val.compare( command::getConfigDaemon ) == 0 )
          {
            //
            // config des Daemons
            // evtl noch zugriff beschränken
            //
            *lg << LDEBUG << "CommandGetHandler::getResponse: read daemon config..." << Qt::endl;
            insertDaemonConfig( jsonObj );
            *lg << LDEBUG << "CommandGetHandler::getResponse: read daemon config...OK" << Qt::endl;
          }
          //
          // alle Alermeinstellungen
          //
          else if ( val.compare( command::getCongigAllCmd ) == 0 )
          {
            //
            // alle alarme
            //
            *lg << LDEBUG << "CommandGetHandler::getResponse: read all alert config...";
            for ( const auto &alert : qAsConst( *( config->getAlConfigs() ) ) )
            {
              *lg << LDEBUG << "CommandGetHandler::getResponse: read alert <" << alert.getName() << "> config..." << Qt::endl;
              insertAlertConfig( jsonObj, alert );
            }
            *lg << LDEBUG << "CommandGetHandler::getResponse: read all alert config...OK";
          }
          //
          // verfügbare sound devices
          //
          else if ( val.compare( command::getConfigDevCmd ) == 0 )
          {
            //
            // alle Geräte ausgeben
            //
            *lg << LDEBUG << "CommandGetHandler::getResponse: read all availible devices...";
            insertAvailDevices( jsonObj );
          }
          //
          // neuen alarm einrichten?
          //
          else if ( val.compare( command::getConfigNewAlCmd ) == 0 )
          {
            *lg << LDEBUG << "CommandGetHandler::getResponse: new alert requested...";
            AlertListPtr alPtr = config->getAlConfigs();
            QList< int > alNum;
            // was ist schon da?
            for ( auto alIt = alPtr->begin(); alIt != alPtr->end(); alIt++ )
            {
              //
              // reduziere auf Zahl und mache die in die Liste
              int number = alIt->getName().remove( config->getAlertPattern() ).toInt();
              alNum << number;
            }
            for ( int num = 0; num < 99; num++ )
            {
              if ( !alNum.contains( num ) )
              {
                insertNewAlertNum( jsonObj, num );
                break;
              }
            }
          }
          //
          // einen bestimmten Alarm lesen?
          //
          else if ( val.contains( config->getAlertPattern() ) )
          {
            //
            // eine konfig eines alarmes soll ausgegeben werden
            //
            *lg << LDEBUG << "CommandGetHandler::getResponse: read config for alert <" << val << ">..." << Qt::endl;
            for ( const auto &alert : qAsConst( *( config->getAlConfigs() ) ) )
            {
              if ( val.compare( alert.getName() ) == 0 )
              {
                *lg << LDEBUG << "CommandGetHandler::getResponse: read config for alert <" << val << ">...OK" << Qt::endl;
                insertAlertConfig( jsonObj, alert );
                break;
              }
            }
          }
          //
          // nichts von allem
          //
          else
          {
            *lg << LWARN << "CommandGetHandler::getResponse: unknown get request: <" << val << ">" << Qt::endl;
          }
        }
        else
        {
          // TODO: ganz böse, was melden oder so
          // ist es ein Objekt und ist es ein GET Objekt? ==> NEIN!
          *lg << LCRIT << "CommandGetHandler::getResponse: unknown request, should be \"get\" with params" << Qt::endl;
          answer = ConnectionHandler::getJSONErrorMessage( "not a valid get json object recived" );
          return answer;
        }
      }
      if ( jsonObj->isEmpty() )
      {
        *lg << LWARN << "CommandGetHandler::getResponse: no parameters for answer includet!";
        answer = ConnectionHandler::getJSONErrorMessage( "no answers for question computed! call programmer!" );
        return answer;
      }
      //
      // erzeuge einen Zeiger auf ein gültiges JSON Stringobjekt
      // zum versenden an den Clienten
      //
      QJsonDocument doc( *jsonObj );
      answer = JSonStringPtr( new QString( doc.toJson( jsonFormat ) ) );
    }
    else
    {
      answer = ConnectionHandler::getJSONErrorMessage( "not a valid json object recived" );
    }
    return answer;
  }

  /**
   * @brief CommandGetHandler::insertDaemonConfig
   * @param jsonObj
   * @return
   */
  void CommandGetHandler::insertDaemonConfig( JsonObjSPtr jsonObj )
  {
    QJsonObject daemonConfig;
    // zusammentragen
    daemonConfig.insert( command::confAppLogfile, config->getLogFileName() );
    daemonConfig.insert( command::confAppConfigFile, config->getConfigFile() );
    daemonConfig.insert( command::confAppBindAddr, config->getBindaddr() );
    daemonConfig.insert( command::confAppBindPort, config->getBindport() );
    daemonConfig.insert( command::confAppWsPort, config->getDefaultWsPort() );
    daemonConfig.insert( command::confAppHttpPort, config->getDefaultHttpPort() );
    daemonConfig.insert( command::confAppThreshold, Logger::getThresholdString( config->getThreshold() ).toLower() );
    daemonConfig.insert( command::confAppDebug, config->getIsDebug() ? QLatin1String( "true" ) : QLatin1String( "false" ) );
    // eintragen in das Objekt
    jsonObj->insert( command::getConfigDaemon, daemonConfig );
  }

  /**
   * @brief CommandGetHandler::insertAlertConfig
   * @param jsonObj
   * @param alert
   */
  void CommandGetHandler::insertAlertConfig( JsonObjSPtr jsonObj, const SingleAlertConfig &alert )
  {
    QJsonObject alertConfig;
    // zusammentragen
    alertConfig.insert( command::confAlertVolume, QString::number( static_cast< int >( alert.getAlVolume() ) ) );
    alertConfig.insert( command::confAlertDate, alert.getAlDate().toString( command::dateFormatStr ) );
    alertConfig.insert( command::confAlertTime, alert.getAlTime().toString( command::timeFormatStr ) );
    alertConfig.insert( command::confAlertLocation, alert.getAlLocation() );
    alertConfig.insert( command::confAlertSource, alert.getAlSource() );
    alertConfig.insert( command::confAlertRaiseVol, alert.getAlRaiseVolume() ? QLatin1String( "true" ) : QLatin1String( "false" ) );
    alertConfig.insert( command::confAlertDuration, QString::number( static_cast< int >( alert.getAlDuration() ) ) );
    alertConfig.insert( command::confAlertSourceAccount, alert.getAlSourceAccount() );
    alertConfig.insert( command::confAlertDays, alert.getAlDaysStr().join( ',' ) );
    alertConfig.insert( command::confAlertDevices, alert.getAlDevices().join( ',' ) );
    alertConfig.insert( command::confAlertType, alert.getAlType() );
    alertConfig.insert( command::confAlertEnabled, alert.getAlEnable() ? QLatin1String( "true" ) : QLatin1String( "false" ) );
    alertConfig.insert( command::confAlertNote, alert.getAlNote() );
    // eintragen in das Objekt
    jsonObj->insert( alert.getName(), alertConfig );
  }

  /**
   * @brief CommandGetHandler::insertAvailDevices
   * @param jsonObj
   */
  void CommandGetHandler::insertAvailDevices( JsonObjSPtr jsonObj )
  {
    QJsonObject devices;
    // zusammentragen
    for ( SoundTouchDevice &dev : config->getAvailDevices() )
    {
      devices.insert( dev.getName(), QLatin1String( "true" ) );
    }
    // eintragen in das Objekt
    jsonObj->insert( "devices", devices );
  }

  void CommandGetHandler::insertNewAlertNum( JsonObjSPtr jsonObj, int num )
  {
    QJsonObject newAlertNum;
    // zusammentragen
    newAlertNum.insert( QString( "%1%2" ).arg( config->getAlertPatternStr() ).arg( num ), QString( "%1" ).arg( num ) );
    // eintragen in das Objekt
    jsonObj->insert( command::getConfigNewAlCmd, newAlertNum );
  }
}  // namespace bose_commserver
