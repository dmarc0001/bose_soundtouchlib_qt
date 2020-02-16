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
    lg->debug( "CommandGetHandler::CommandGetHandler: create object..." );
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
    lg->debug( "CommandGetHandler::CommandGetHandler: get answer..." );
    //
    // ist es ein Objekt und ist es ein GET Objekt?
    //
    if ( !gRequest[ "get" ].isNull() && gRequest[ "get" ].isObject() )
    {
      JsonObjSPtr jsonObj = JsonObjSPtr( new QJsonObject() );
      //
      // gib ma das anforderungs Array (da Aray mit den get-Anforderungen)
      //
      lg->debug( "CommandGetHandler::CommandGetHandler: get request is acknolowedged..." );
      //
      // eine map draus machen, die kann ich gut abarbeiten
      //
      QVariantMap getMap = gRequest[ "get" ].toObject().toVariantMap();
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
        lg->debug( QString( "CommandGetHandler::CommandGetHandler: get params: <%1> <%2>..." ).arg( key ).arg( val ) );
        //
        // Config anfrage?
        //
        if ( key.compare( "config" ) == 0 )
        {
          //
          // was will er wissen?
          //
          if ( val.compare( "daemon" ) == 0 )
          {
            //
            // config des Daemons
            // evtl noch zugriff beschränken
            //
            lg->debug( "CommandGetHandler::CommandGetHandler: read daemon config..." );
            insertDaemonConfig( jsonObj );
            lg->debug( "CommandGetHandler::CommandGetHandler: read daemon config...OK" );
          }
          else if ( val.compare( "all" ) == 0 )
          {
            //
            // alle alarme
            //
            lg->debug( "CommandGetHandler::CommandGetHandler: read all alert config..." );
            for ( const auto &alert : *( config->getAlConfigs() ) )
            {
              lg->debug( QString( "CommandGetHandler::CommandGetHandler: read alert %1 config..." ).arg( alert.getName() ) );
              insertAlertConfig( jsonObj, alert );
            }
            lg->debug( "CommandGetHandler::CommandGetHandler: read all alert config...OK" );
          }
          else if ( val.compare( "devices" ) == 0 )
          {
            //
            // alle Geräte ausgeben
            //
            lg->debug( "CommandGetHandler::CommandGetHandler: read all availible devices..." );
            insertAvailDevices( jsonObj );
          }
          else if ( val.contains( config->getAlertPattern() ) )
          {
            //
            // eine konfig eines alarmes soll ausgegeben werden
            //
            lg->debug( QString( "CommandGetHandler::CommandGetHandler: read config for alert %1..." ).arg( val ) );
            for ( const auto &alert : *( config->getAlConfigs() ) )
            {
              if ( val.compare( alert.getName() ) == 0 )
              {
                lg->debug( QString( "CommandGetHandler::CommandGetHandler: read config for alert %1...OK" ).arg( val ) );
                insertAlertConfig( jsonObj, alert );
                break;
              }
            }
          }
          else
          {
            lg->warn( QString( "CommandGetHandler::getResponse: unknown get request: %1" ).arg( val ) );
          }
        }
        else
        {
          // TODO: ganz böse, was melden oder so
          // ist es ein Objekt und ist es ein GET Objekt? ==> NEIN!
          lg->crit( "CommandGetHandler::getResponse: unknown request, should be \"get\" with params" );
          answer = getJSONErrorMessage( "not a valid get json object recived" );
          return answer;
        }
      }
      if ( jsonObj->isEmpty() )
      {
        lg->warn( "CommandGetHandler::getResponse: no parameters for answer includet!" );
        answer = getJSONErrorMessage( "no answers for question computed! call programmer!" );
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
      answer = getJSONErrorMessage( "not a valid json object recived" );
    }
    return answer;
  }

  /**
   * @brief CommandGetHandler::getJSONErrorMessage
   * @param errormsg
   * @return
   */
  JSonStringPtr CommandGetHandler::getJSONErrorMessage( const QString &errormsg )
  {
    // das Objekt zum Übergeben
    JsonObjSPtr jsonObj = JsonObjSPtr( new QJsonObject() );
    // Das Array für Fehlermeldung
    QJsonObject errArray;
    errArray.insert( "msg", errormsg );
    jsonObj->insert( "error", errArray );
    QJsonDocument doc( *jsonObj );
    JSonStringPtr strJson = JSonStringPtr( new QString( doc.toJson( jsonFormat ) ) );
    return strJson;
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
    daemonConfig.insert( "logfilename", config->getLogFileName() );
    daemonConfig.insert( "configfilename", config->getConfigFile() );
    daemonConfig.insert( "bindaddr", config->getBindaddr() );
    daemonConfig.insert( "bindport", config->getBindport() );
    daemonConfig.insert( "wsport", config->getDefaultWsPort() );
    daemonConfig.insert( "httpport", config->getDefaultHttpPort() );
    daemonConfig.insert( "threshold", QString( static_cast< int >( config->getThreshold() ) ) );
    daemonConfig.insert( "debug", config->getIsDebug() ? QLatin1String( "true" ) : QLatin1String( "false" ) );
    // eintragen in das Objekt
    jsonObj->insert( "daemon", daemonConfig );
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
    alertConfig.insert( "volume", QString::number( static_cast< int >( alert.getAlVolume() ) ) );
    alertConfig.insert( "date", alert.getAlDate().toString( "yyyy-MM-dd" ) );
    alertConfig.insert( "time", alert.getAlTime().toString( "HH:mm" ) );
    alertConfig.insert( "location", alert.getAlLocation() );
    alertConfig.insert( "source", alert.getAlSource() );
    alertConfig.insert( "raise_vol", alert.getAlRaiseVolume() ? QLatin1String( "true" ) : QLatin1String( "false" ) );
    alertConfig.insert( "duration", QString::number( static_cast< int >( alert.getAlDuration() ) ) );
    alertConfig.insert( "source_account", alert.getAlSourceAccount() );
    alertConfig.insert( "days", alert.getAlDaysStr().join( ',' ) );
    alertConfig.insert( "devices", alert.getAlDevices().join( ',' ) );
    alertConfig.insert( "type", alert.getAlType() );
    alertConfig.insert( "enabled", alert.getAlEnable() ? QLatin1String( "true" ) : QLatin1String( "false" ) );
    alertConfig.insert( "note", alert.getAlNote() );
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
    for ( QString &dev : config->getAvailDevices() )
    {
      devices.insert( dev, QLatin1String( "true" ) );
    }
    // eintragen in das Objekt
    jsonObj->insert( "devices", devices );
  }
}  // namespace bose_commserver
