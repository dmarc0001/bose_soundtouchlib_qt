#include "main.hpp"
#include <QDebug>

using namespace testclient;

int main( int argc, char *argv[] )
{
  QCoreApplication app( argc, argv );

  QCommandLineParser parser;
  parser.setApplicationDescription( "Bose library: testclient" );
  parser.addHelpOption();
  //
  // bindaddr fuer den Daemon, hier lauscht der
  //
  QCommandLineOption urlOption( "url", QString( "connect url [default: ws://localhost:8080]" ), "destination", "ws://localhost:8080" );
  //
  // Optionen parsen und einlesen
  //
  parser.addOption( urlOption );
  parser.process( app );
  QUrl url = QUrl( parser.value( urlOption ) );
  //
  // Client instanzieren, ENDE Signal verbinden
  //
  TestClient client( url );
  QObject::connect( &client, &TestClient::closed, &app, &QCoreApplication::quit );
  //
  // auf das Ende warten
  //
  return app.exec();
}
