#include <QApplication>
#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include "librarytestwindow.hpp"

#include <QDebug>

int main( int argc, char *argv[] )
{
  QCommandLineParser parser;
  QApplication a( argc, argv );
  //
  parser.setApplicationDescription( "soundtouch: Tests" );
  parser.addHelpOption();
  //
  // debug an oder aus
  //
  QCommandLineOption dbgOption( QStringList() << "d"
                                              << "debug",
                                QCoreApplication::translate( "main", "Debug output [default: off]." ) );
  //
  // ws port einstellen (default 8090)
  //
  QCommandLineOption wsPortOption( QStringList() << "w"
                                                 << "ws-port",
                                   QCoreApplication::translate( "main", "Port for websocket [default: 8080]." ),
                                   QCoreApplication::translate( "main", "ws-port" ),
                                   QLatin1String( bose_soundtoch_lib::ProgramConfig::wsport ) );
  //
  // ws port einstellen (default 8080)
  //
  QCommandLineOption httpPortOption( QStringList() << "p"
                                                   << "http-port",
                                     QCoreApplication::translate( "main", "Port for httpsocket [default: 8090]." ),
                                     QCoreApplication::translate( "main", "http-port" ),
                                     QLatin1String( bose_soundtoch_lib::ProgramConfig::httpport ) );
  //
  // url lesen (default localhost)
  //
  QCommandLineOption hostOption( QStringList() << "t"
                                               << "host",
                                 QCoreApplication::translate( "main", "host for client [default: localhost]." ),
                                 QCoreApplication::translate( "main", "host" ),
                                 QLatin1String( bose_soundtoch_lib::ProgramConfig::server ) );

  parser.addOption( dbgOption );
  parser.addOption( wsPortOption );
  parser.addOption( httpPortOption );
  parser.addOption( hostOption );
  parser.process( a );
  bool debug = parser.isSet( dbgOption );
  int wsPort = parser.value( wsPortOption ).toInt();
  int httpPort = parser.value( httpPortOption ).toInt();
  QString hostname = parser.value( hostOption );
  bose_soundtoch_lib::LibraryTestWindow w( hostname, wsPort, httpPort, debug );
  w.show();

  return a.exec();
}
