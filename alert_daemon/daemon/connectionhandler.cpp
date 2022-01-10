#include "connectionhandler.hpp"

#include <QDebug>

#include "bosecommserver.hpp"

namespace bose_commserver
{
  qlonglong ConnectionHandler::handlerNumberCounter{0};

  /**
   * @brief ConnectionHandler::ConnectionHandler
   * @param dconfig
   * @param theSock
   * @param parent
   */
  ConnectionHandler::ConnectionHandler( AppConfigPtr dconfig, std::shared_ptr< QWebSocket > theSock, QObject *parent )
      : QObject( parent )
      , currentHandlerNum( ConnectionHandler::handlerNumberCounter++ )
      , config( dconfig )
      , connected( false )
      , nSock( theSock )
      , lg( dconfig->getLogger() )
  {
    //
    *lg << LDEBUG
        << QString( "ConnectionHandler::ConnectionHandler: created handler id: %1" )
               .arg( currentHandlerNum, 12, 10, QLatin1Char( '0' ) )
        << Qt::endl;
    init();
  }

  /**
   * @brief ConnectionHandler::ConnectionHandler
   * @param cp
   */
  ConnectionHandler::ConnectionHandler( const ConnectionHandler &cp ) : currentHandlerNum( cp.currentHandlerNum )
  {
    *lg << LDEBUG
        << QString( "ConnectionHandler::ConnectionHandler: copyconstructor created handler id: %1" )
               .arg( currentHandlerNum, 12, 10, QLatin1Char( '0' ) )
        << Qt::endl;
    config = cp.config;
    connected = cp.connected;
    nSock = cp.nSock;
    init();
  }

  /**
   * @brief ConnectionHandler::init
   */
  void ConnectionHandler::init()
  {
    *lg << LDEBUG << "ConnectionHandler::init: connect slots for websocket..." << Qt::endl;
    connect( nSock.get(), &QWebSocket::textMessageReceived, this, &ConnectionHandler::onRecTextMessage );
    connect( nSock.get(), &QWebSocket::binaryMessageReceived, this, &ConnectionHandler::onRecBinaryMessage );
    connect( nSock.get(), &QWebSocket::disconnected, this, &ConnectionHandler::onDisconnected );
  }

  /**
   * @brief ConnectionHandler::getNSock
   * @return
   */
  std::shared_ptr< QWebSocket > ConnectionHandler::getNSock() const
  {
    return nSock;
  }

  /**
   * @brief ConnectionHandler::~ConnectionHandler
   */
  ConnectionHandler::~ConnectionHandler()
  {
    *lg << LDEBUG
        << QString( "ConnectionHandler::~ConnectionHandler: destruction handler id: %1" )
               .arg( currentHandlerNum, 12, 10, QLatin1Char( '0' ) )
        << Qt::endl;
    /*
    // Websocket verbindung beenden
    if ( nSock->isValid() )
      if ( connected )
      {
        nSock->close();
        connected = false;
      }
    nSock->deleteLater();
    */
  }

  /**
   * @brief ConnectionHandler::getCurrentHandler
   * @return
   */
  qlonglong ConnectionHandler::getCurrentHandlerNum() const
  {
    return currentHandlerNum;
  }

  /**
   * @brief ConnectionHandler::disconnectWebsocket
   */
  void ConnectionHandler::disconnectWebsocket()
  {
    *lg << LDEBUG
        << QString( "ConnectionHandler::disconnectWebsocket: disconnect handler id: %1" )
               .arg( currentHandlerNum, 12, 10, QLatin1Char( '0' ) )
        << Qt::endl;
    // Websocket verbindung beenden
    if ( nSock->isValid() )
      if ( connected )
      {
        nSock->close();
      }
    connected = false;
    nSock = nullptr;
  }

  /**
   * @brief ConnectionHandler::onRecTextMessage
   * @param msg
   */
  void ConnectionHandler::onRecTextMessage( QString msg )
  {
    *lg << LDEBUG
        << QString( "ConnectionHandler::onRecTextMessage: handler id: %1 recived: %2 from %3" )
               .arg( currentHandlerNum, 12, 10, QLatin1Char( '0' ) )
               .arg( msg )
               .arg( nSock->peerAddress().toString() )
        << Qt::endl;
    JSonStringPtr answer;
    //
    // ein JSON Dokument aus der Nachricht erzeugen
    //
    QJsonDocument doc = QJsonDocument::fromJson( msg.toUtf8() );
    if ( doc.isNull() )
    {
      //
      // Fehler bei der Erstellung
      //
      *lg << LWARN << "ConnectionHandler::onRecTextMessage: create json object failed! Ignore!" << Qt::endl;
      //
      // sende Fehlermeldung
      //
      answer = getJSONErrorMessage( "not a valid json recived" );
      nSock->sendTextMessage( *answer );
      return;
    }
    // das jsonObject holen
    QJsonObject jObject = doc.object();
    QStringList keys = jObject.keys();
    if ( keys.contains( command::getCmd ) )
    {
      //
      // get kommando an das richtige objekt übergeben
      // wird relativ selten gebraucht, daher temporär erzeugt und dann verworfen
      //
      CommandGetHandler getHandler( config, jObject );
      answer = getHandler.getResponse();
    }
    else if ( keys.contains( command::setCmd ) )
    {
      // set kommando
      CommandSetHandler getHandler( config, jObject );
      answer = getHandler.getResponse();
    }
    else if ( keys.contains( command::deleteCmd ) )
    {
      // delete kommando
      answer = getJSONErrorMessage( "delete command" );
    }
    else
    {
      // Fehlermeldung senden
      *lg << LWARN << "ConnectionHandler::onRecTextMessage: unknown command recived!" << Qt::endl;
      answer = getJSONErrorMessage( "unknown command" );
    }
    //
    // Antwort senden
    //
    nSock->sendTextMessage( *answer );
  }

  /**
   * @brief ConnectionHandler::getJSONErrorMessage
   * @param errormsg
   * @return
   */
  JSonStringPtr ConnectionHandler::getJSONErrorMessage( const QString &errormsg )
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
   * @brief ConnectionHandler::onRecBinaryMessage
   */
  void ConnectionHandler::onRecBinaryMessage( QByteArray msg )
  {
    *lg << LDEBUG
        << QString( "ConnectionHandler::onRecTextMessage: handler id: %1 recived: %2 from %3" )
               .arg( currentHandlerNum, 12, 10, QLatin1Char( '0' ) )
               .arg( QString( msg.toHex( ':' ) ) )
               .arg( nSock->peerAddress().toString() )
        << Qt::endl;
    nSock->sendTextMessage( "BINARY ACKNOWLEDGED" );
  }

  /**
   * @brief ConnectionHandler::onDisconnected
   */
  void ConnectionHandler::onDisconnected()
  {
    *lg << LDEBUG << "ConnectionHandler: in disconnected handler id: " << currentHandlerNum << Qt::endl;
    //
    // wer war das?
    //
    *lg << LDEBUG << "commserver: closed remote connection from: " << nSock->peerAddress().toString() << ", " << nSock->peerPort()
        << Qt::endl;
    //
    // gib bei der App bescheid, dass alles erledigt ist
    // => Programm beenden
    //
    emit closed( this );
  }

}  // namespace bose_commserver
