#include "commandsethandler.hpp"

namespace bose_commserver
{
  CommandSetHandler::CommandSetHandler( AppConfigPtr dconfig, QJsonObject request ) : config( dconfig ), gRequest( request )
  {
    lg = config->getLogger();
    lg->debug( "CommandSetHandler::CommandSetHandler: create object..." );
  }

  JSonStringPtr CommandSetHandler::getResponse()
  {
    JSonStringPtr answer;
    //
    // es ist ein set Kommando, wurde vor der Instanzierung geprüft
    //
    lg->debug( "CommandSetHandler::getResponse: get answer..." );
    //
    // ist es ein Objekt und ist es ein SET Objekt?
    //
    if ( !gRequest[ command::setCmd ].isNull() && gRequest[ command::setCmd ].isObject() )
    {
      JsonObjSPtr jsonObj = JsonObjSPtr( new QJsonObject() );
      //
      // gib ma das objekt mit den Anweisungen
      //
      lg->debug( "CommandSetHandler::getResponse: set request is acknolowedged..." );
      //
      // eine map draus machen, die kann ich gut abarbeiten
      //
      QVariantMap setMap = gRequest[ command::setCmd ].toObject().toVariantMap();
      //
      // arbeite die Map ab, jedes Element
      //
      for ( auto toSetIt = setMap.begin(); toSetIt != setMap.end(); toSetIt++ )
      {
        //
        // es können nur String/Objekct da sein
        //
        QString key = toSetIt.key();
        QJsonObject val = toSetIt.value().toJsonObject();
        lg->debug( QString( "CommandSetHandler::getResponse: set type: <%1>..." ).arg( key ) );
        //
        // daemon set Config anfrage?
        // TODO: evtl noch zugriff beschränken
        //
        if ( key.compare( command::setConfigDaemon ) == 0 )
        {
          //
          // Alle Objekte durchgehen, und sehen was er will
          //
          QVariantMap setDetailMap = val.toVariantMap();
          for ( auto daemonIter = setDetailMap.begin(); daemonIter != setDetailMap.end(); daemonIter++ )
          {
            //
            // loglevel des daemons
            //
            if ( daemonIter.key().compare( command::confAppThreshold ) )
            {
              QString setValName = daemonIter.key();
              QString setVal = daemonIter.value().toString();

              LgThreshold thr = Logger::getThresholdFromString( setVal );
              config->setThreshold( thr );
              lg->debug( QString( "CommandSetHandler::getResponse: set daemon config to %1..." ).arg( setVal ) );
              config->setIsDebug( setVal.compare( "true" ) == 0 ? true : false );
              insertOkFor( jsonObj, daemonIter.key(), true );
            }
            //
            // TODO: bei Bedarf mehr Eigenschaften einstllen
            // else if ( iter.key().compare( "eigenschaft" ) )
          }
        }
        //
        // Alarme konfigurieren
        //
        else if ( key.contains( config->getAlertPattern() ) )
        {
          //
          // Alarm einstellen der Alarmname ist key
          // alle Elemente als MAP konvertieren
          //
          QVariantMap setSingleAlertMap = val.toVariantMap();
          //
          // liste auf alarmconfigs, alle vorhandenen Alarme sind dort
          //
          AlertListPtr alPtr = config->getAlConfigs();
          //
          // suche, ob der gewünschte alarm key in den Configs vorhanden ist
          //
          SingleAlertConfig &sAlert = findAlert( key );
          if ( sAlert.getName().compare( NONE_AL ) == 0 )
          {
            //
            // nix gefunden, neu machen, wenn attribute "new-alert" drin ist
            //
            if ( setSingleAlertMap.contains( command::setConfigNewAlCmd ) )
            {
              // erzeuge einen neuen Alarm
              //
              SingleAlertConfig newAlert( key );
              updateSingleAlert( sAlert, setSingleAlertMap );
              alPtr->append( newAlert );
              // insertOkFor( jsonObj, key, true );
            }
            else
            {
              // insertOkFor( jsonObj, key, false );
              lg->warn( "CommandSetHandler::getResponse: trying to configure an not exist alert! ignore!" );
            }
          }
          else
          {
            updateSingleAlert( sAlert, setSingleAlertMap );
            // insertOkFor( jsonObj, key, true );
          }
        }  // ende alerts
        //
        // alles was sonst wäre
        // (sollte eigentlich nicht vorkommen
        //
        else
        {
          //
          // nix zum einstellen gefunden
          //
          lg->crit( "CommandSetHandler::getResponse: unknown request, should be \"set\" with params" );
          answer = ConnectionHandler::getJSONErrorMessage( "not a valid get json object recived" );
          return answer;
        }
      }  // ende for schleife
      if ( jsonObj->isEmpty() )
      {
        //
        // keine Antworten gefunden?
        //
        lg->warn( "CommandSetHandler::getResponse: no parameters for answer includet!" );
        answer = ConnectionHandler::getJSONErrorMessage( "no answers for set request computed! call programmer!" );
        return answer;
      }
    }  // ende if request
    else
    {
      //
      // kein gültiges set objekt vorhanden
      //
      answer = ConnectionHandler::getJSONErrorMessage( "not a valid json object recived" );
    }
    return answer;
  }

  /**
   * @brief CommandSetHandler::insertOkFor
   * @param jsonObj
   * @param key
   * @param isOk
   */
  void CommandSetHandler::insertOkFor( JsonObjSPtr jsonObj, const QString &key, bool isOk )
  {
    // eintragen in das Objekt
    jsonObj->insert( key, isOk ? QLatin1String( "true" ) : QLatin1String( "false" ) );
  }

  /**
   * @brief CommandSetHandler::findAlert
   * @param key
   * @return
   */
  SingleAlertConfig &CommandSetHandler::findAlert( const QString &key )
  {
    static SingleAlertConfig nope( NONE_AL );

    AlertListPtr alPtr = config->getAlConfigs();

    for ( auto alertListIter = alPtr->begin(); alertListIter != alPtr->end(); alertListIter++ )
    {
      //
      // einen vorhandenen Alarm gefunden?
      //
      if ( alertListIter->getName().compare( key ) == 0 )
      {
        return *alertListIter;
      }
    }
    return nope;
  }

  void CommandSetHandler::updateSingleAlert( SingleAlertConfig &sAlert, const QVariantMap &setSigleAlertMap )
  {
    //
    // ja, diesen meine ich
    // jetzt alle werte aus dem zugehörigen Objekt machen
    //
    for ( auto singleAlertIter = setSigleAlertMap.begin(); singleAlertIter != setSigleAlertMap.end(); singleAlertIter++ )
    {
      QString alertConfigKey = singleAlertIter.key();
      QString alertConfigValue = singleAlertIter.value().toString();
      //
      if ( alertConfigKey.compare( command::confAlertVolume ) == 0 )
      {
        sAlert.setAlVolume( static_cast< qint8 >( alertConfigValue.toInt() ) );
      }
      else if ( alertConfigKey.compare( command::confAlertDate ) == 0 )
      {
        sAlert.setAlDate( QDate::fromString( alertConfigValue, command::dateFormatStr ) );
      }
      else if ( alertConfigKey.compare( command::confAlertTime ) == 0 )
      {
        sAlert.setAlTime( QTime::fromString( alertConfigValue, command::timeFormatStr ) );
      }
      else if ( alertConfigKey.compare( command::confAlertLocation ) == 0 )
      {
        sAlert.setAlLocation( alertConfigValue );
      }
      else if ( alertConfigKey.compare( command::confAlertSource ) == 0 )
      {
        sAlert.setAlSource( alertConfigValue );
      }
      else if ( alertConfigKey.compare( command::confAlertRaiseVol ) == 0 )
      {
        sAlert.setAlRaiseVolume( alertConfigValue.compare( "true" ) == 0 ? true : false );
      }
      else if ( alertConfigKey.compare( command::confAlertDuration ) == 0 )
      {
        sAlert.setAlDuration( static_cast< qint16 >( alertConfigValue.toInt() ) );
      }
      else if ( alertConfigKey.compare( command::confAlertSourceAccount ) == 0 )
      {
        sAlert.setAlSourceAccount( alertConfigValue );
      }
      else if ( alertConfigKey.compare( command::confAlertDays ) == 0 )
      {
        QList< qint8 > list;
        QStringList days = alertConfigValue.split( "," );
        for ( QString day : days )
        {
          if ( day.trimmed().compare( command::weekdays[ 1 ] ) )
            list << 1;
          else if ( day.trimmed().compare( command::weekdays[ 2 ] ) )
            list << 2;
          else if ( day.trimmed().compare( command::weekdays[ 3 ] ) )
            list << 3;
          else if ( day.trimmed().compare( command::weekdays[ 4 ] ) )
            list << 4;
          else if ( day.trimmed().compare( command::weekdays[ 5 ] ) )
            list << 5;
          else if ( day.trimmed().compare( command::weekdays[ 6 ] ) )
            list << 6;
          else
            list << 7;
        }
        sAlert.setAlDays( list );
      }
      else if ( alertConfigKey.compare( command::confAlertDevices ) == 0 )
      {
        QStringList devices = alertConfigValue.split( "," );
        sAlert.setAlDevices( devices );
      }
      else if ( alertConfigKey.compare( command::confAlertType ) == 0 )
      {
        sAlert.setAlType( alertConfigValue );
      }
      else if ( alertConfigKey.compare( command::confAlertNote ) == 0 )
      {
        sAlert.setAlNote( alertConfigValue );
      }
      else if ( alertConfigKey.compare( command::confAlertEnabled ) == 0 )
      {
        sAlert.setAlEnable( alertConfigValue.compare( "true" ) == 0 ? true : false );
      }
      else if ( alertConfigKey.compare( command::setConfigNewAlCmd ) == 0 )
      {
        lg->debug( "CommandSetHandler::updateSingleAlert: this is an new alert..." );
      }
      else
      {
        lg->warn( QString( "CommandSetHandler::updateSingleAlert: unknown set for %1 recived: %2" )
                      .arg( sAlert.getName() )
                      .arg( alertConfigKey ) );
      }
    }
  }
}  // namespace bose_commserver
