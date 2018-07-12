#include "xmlupdateparser.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief XMLUpdateParser::XMLUpdateParser
   * @param xmlString
   * @param parent
   */
  XMLUpdateParser::XMLUpdateParser( QString &xmlString, QObject *parent )
      : QObject( parent ), wasError( false ), errMsg( "" ), errLine( 0 ), errColumn( 0 )
  {
    qDebug() << "start update parsing xml string...";
    if ( !document.setContent( xmlString, &errMsg, &errLine, &errColumn ) )
    {
      qCritical().noquote() << "error while parsing xml on line" << errLine << "col:" << errColumn << "(" << errMsg << ")";
      wasError = true;
      //
      // TODO: Errorhandling
      //
    }
    //
    // das oberste element im Baum suchen
    //
    docElem = document.documentElement();
    parseDom();
  }

  /**
   * @brief XMLUpdateParser::~XMLUpdateParser
   */
  XMLUpdateParser::~XMLUpdateParser()
  {
    qDebug() << "...";
  }

  /**
   * @brief XMLUpdateParser::parseFile das XML parsen und typisieren
   * @return
   */
  void XMLUpdateParser::parseDom( void )
  {
    QString rootelemName = docElem.tagName();
    //
    // zuordnung machen, IResponseObject ist polymorph, das erleichtert die Handhabung
    // SDK-Info : <SoundTouchSdkInfo serverVersion="4" serverBuild="trunk r42017 v4 epdbuild cepeswbld02" />
    //
    if ( rootelemName == QLatin1String( "SoundTouchSdkInfo" ) )
    {
      //
      // SDK Info (beim Verbinden übertragen
      // erzeuge das Objekt und Parse es
      //
      responseObject = std::shared_ptr< IResponseObject >( new WsSoundTouchSdkInfo( &docElem, this ) );
    }
    else if ( rootelemName == QLatin1String( "userActivityUpdate" ) )
    {
      //
      // Benutzer aktiv geändert
      // Activiti update ist NICHT in <updates>
      responseObject = std::shared_ptr< IResponseObject >( new WsUserActivityUpdated( &docElem, this ) );
      // responseObject->setDeviceId( deviceId );
    }
    else if ( rootelemName == QLatin1String( "updates" ) )
    {
      QString deviceId = IResponseObject::getAttribute( &docElem, QLatin1String( "deviceID" ) );
      //
      // eine UPDATE Meldung, sollte alles sein ausser SoundTouchSdkInfo
      // Liste mit allen Knoten. Solle eignetlich nur eine Meldung sein, aber man kann ja nie wissen ;-)
      //
      QDomNodeList updateList( docElem.childNodes() );
      for ( int nodeIdx = 0; nodeIdx < updateList.length(); nodeIdx++ )
      {
        QDomElement currElem( updateList.item( nodeIdx ).toElement() );
        if ( currElem.isNull() )
          continue;
        //
        // unterscheide die Knoten
        // der Name ist hier als QString / nodeName == tagNeme
        //
        QString updateElementName( currElem.nodeName() );
        //
        // jetzt entscheidet sich welche Art UPDATE empfangen wurde
        //
        if ( updateElementName == QLatin1String( "presetsUpdates" ) )
        {
          responseObject = std::shared_ptr< IResponseObject >( new WsPresetUpdateObject( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // nowPlayingUpdate
        else if ( updateElementName == QLatin1String( "nowPlayingUpdated" ) )
        {
          //
          // ich kann hier das HttpNowPlayingObjekt recyclen (via "using" in der Headerdatei)
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsNowPlayingUpdate( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // nowSelectionUpdated
        else if ( updateElementName == QLatin1String( "nowSelectionUpdated" ) )
        {
          //
          // Selection geändert
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsNowSelectionUpdated( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // volume geändert
        else if ( updateElementName == QLatin1String( "volumeUpdated" ) )
        {
          //
          // Lautstärke geändert
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsVolumeUpdated( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // Bass geändert
        else if ( updateElementName == QLatin1String( "bassUpdated" ) )
        {
          //
          // Bass geändert
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsBassUpdated( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // Zone geändert
        else if ( updateElementName == QLatin1String( "zoneUpdated" ) )
        {
          //
          // Zonenmitgliedschaft geändert
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsZoneUpdated( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // Info geändert
        else if ( updateElementName == QLatin1String( "infoUpdated" ) )
        {
          //
          // Geräteinfo geändert
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsInfoUpdated( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // Gerätename geändert
        else if ( updateElementName == QLatin1String( "nameUpdated" ) )
        {
          //
          // Gerätename geändert
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsNameUpdated( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // error update
        else if ( updateElementName == QLatin1String( "errorUpdate" ) )
        {
          //
          // Fehlermeldung geändert
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsErrorUpdated( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // goup update
        else if ( updateElementName == QLatin1String( "groupUpdated" ) )
        {
          //
          // Gruppe (SounfTouch 10 only) geändert
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsGroupUpdated( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        //###################################################################
        // UNSUPPORTED Features
        //###################################################################
        // browse
        else if ( updateElementName == QLatin1String( "browseUpdated" ) )
        {
          //
          // Browsing update
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsBrowseUpdated( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // recents
        else if ( updateElementName == QLatin1String( "recentsUpdated" ) )
        {
          //
          // letzte Medien update
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsRecentsUpdated( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // source updated
        else if ( updateElementName == QLatin1String( "sourcesUpdated" ) )
        {
          //
          // Sources update
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsSourceUpdated( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // language update
        else if ( updateElementName == QLatin1String( "languageUpdated" ) )
        {
          //
          // Sprache geändert
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsLanguageUpdated( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // user activity update
        // ist aktuell 2018-07 direkt root tag!
        // OBSOLETE!
        else if ( updateElementName == QLatin1String( "userActivityUpdate" ) )
        {
          //
          // Benutzer aktiv geändert
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsUserActivityUpdated( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // user inactivity
        else if ( updateElementName == QLatin1String( "userInactivityUpdate" ) )
        {
          //
          // Benutzer inaktiv geändert
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsUserInactivityUpdated( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // connection state
        else if ( updateElementName == QLatin1String( "connectionStateUpdated" ) )
        {
          //
          // Verbindungsstatus geändert
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsConnectionStateUpdated( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // audioproducttonecontrols
        else if ( updateElementName == QLatin1String( "audioproducttonecontrols" ) )
        {
          //
          // BOSE intern
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsAudioProductToneControls( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // audioproductlevelcontrols
        else if ( updateElementName == QLatin1String( "audioproductlevelcontrols" ) )
        {
          //
          // BOSE intern
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsAudioProductLevelControls( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        // audiospcontrols
        else if ( updateElementName == QLatin1String( "audiospcontrols" ) )
        {
          //
          // BOSE intern
          //
          responseObject = std::shared_ptr< IResponseObject >( new WsAudioSpControls( &currElem, this ) );
          responseObject->setDeviceId( deviceId );
        }
        //###################################################################
        // UNSUPPORTED Features END
        //###################################################################
        else
        {
          //
          // hier weiss ich auch nicht weiter, der TAG ist unbekannt
          //
          qWarning() << "unsupported TAG behind <updates> in response XML struct found: " << currElem.tagName();
        }
      }
    }
    else
    {
      //
      // hier weiss ich auch nicht weiter, der TAG ist unbekannt
      //
      qWarning() << "unsupported TAG in update response XML struct found: " << rootelemName;
    }
  }

  //
  // GETTER
  //

  /**
   * @brief XMLUpdateParser::hasError Trat ein Fehler beim parsen auf?
   * @return
   */
  bool XMLUpdateParser::hasError( void )
  {
    return ( wasError );
  }

  /**
   * @brief XMLUpdateParser::getErrorString Fehlernachricht zurückgeben, falls Fehler auftrat
   * @return
   */
  QString XMLUpdateParser::getErrorString( void )
  {
    if ( wasError )
    {
      QString errStr( QString( "error %1 at line %2, col %2" ).arg( errMsg ).arg( errLine ).arg( errColumn ) );
      return ( errStr );
    }
    return ( QLatin1String( "no error" ) );
  }

  /**
   * @brief XMLUpdateParser::getResultObject DAs Eregebnisobjekt zurückgeben
   * @return
   */
  std::shared_ptr< IResponseObject > XMLUpdateParser::getResultObject( void )
  {
    return ( responseObject );
  }

}  // namespace bose_soundtoch_lib
