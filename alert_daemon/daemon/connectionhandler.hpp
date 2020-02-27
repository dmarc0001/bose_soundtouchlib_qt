#ifndef CONNECTIONHANDLER_HPP
#define CONNECTIONHANDLER_HPP

#include <QJsonDocument>
#include <QJsonObject>
#include <QLatin1Char>
#include <QList>
#include <QObject>
#include <QWebSocket>
#include <memory>

#include "commandgethandler.hpp"
#include "commandsethandler.hpp"
#include "config/alertconfig.hpp"
#include "config/common_def.hpp"

namespace bose_commserver
{
  class ConnectionHandler;

  using ConnectionHandlerList = QList< std::shared_ptr< ConnectionHandler > >;

  class ConnectionHandler : public QObject
  {
    Q_OBJECT
    private:
    static qlonglong handlerNumberCounter;
    const qlonglong currentHandlerNum;
    AppConfigPtr config;
    bool connected;
    std::shared_ptr< QWebSocket > nSock;
    std::shared_ptr< Logger > lg;  //! wenn ein logger erstellt wurde
    // QJsonDocument::JsonFormat jsonFormat;

    public:
    explicit ConnectionHandler( AppConfigPtr dconfig, std::shared_ptr< QWebSocket > theSock, QObject *parent = nullptr );
    ConnectionHandler( const ConnectionHandler &cp );
    static JSonStringPtr getJSONErrorMessage( const QString &errormsg );
    void init();
    ~ConnectionHandler();
    void disconnectWebsocket();              //! Verbindung trennen
    qlonglong getCurrentHandlerNum() const;  // den aktuellen handler identifizieren
    std::shared_ptr< QWebSocket > getNSock() const;

    private:
    signals:
    void closed( const ConnectionHandler *handler );

    // private slots:
    public slots:
    void onRecTextMessage( QString msg );       //! Kommando als text empfangen
    void onRecBinaryMessage( QByteArray msg );  //! Kommando binär empfangen
    void onDisconnected();                      //! Kommandoverbindung beendet
  };
}  // namespace bose_commserver
#endif  // CONNECTIONHANDLER_HPP
