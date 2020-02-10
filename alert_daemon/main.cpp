#include "main.hpp"

using namespace bose_commserver;

int main( int argc, char *argv[] )
{
  QCoreApplication app( argc, argv );
  QCommandLineParser parser;
  std::shared_ptr< DaemonConfig > dConf = std::shared_ptr< DaemonConfig >( new DaemonConfig() );
  //
  // die Optionen machen
  //
  parser.setApplicationDescription( "commserver for bose soundtouch websocket" );
  parser.addHelpOption();
  //
  // debug option
  //
  QCommandLineOption dbgOption( "debug", "Debug output [default: off]." );
  //
  // Logger Option
  //
  QCommandLineOption logOption( "logfile", QString( "logging file [default: %1]" ).arg( QLatin1String( ProgramConfig::logfile ) ),
                                "filename", QLatin1String( ProgramConfig::logfile ) );
  //
  // bindaddr fuer dieseen Daemon
  //
  QCommandLineOption bindAddrOption( "bind", QString( "bind addr [default: %1]" ).arg( QLatin1String( ProgramConfig::bindaddr ) ),
                                     "local addr", QLatin1String( ProgramConfig::bindaddr ) );
  //
  // bindport fuer diesen daemon
  //
  QCommandLineOption portOption( "port", QString( "bind port [default: %1]" ).arg( QLatin1String( ProgramConfig::bindport ) ),
                                 "local addr", QLatin1String( ProgramConfig::bindport ) );
  parser.addOption( dbgOption );
  parser.addOption( logOption );
  parser.addOption( bindAddrOption );
  parser.addOption( portOption );
  //
  // Auswerten der Optionen
  //
  parser.process( app );
  //
  dConf->setIsDebug( parser.isSet( dbgOption ) );
  if ( parser.isSet( logOption ) )
    dConf->setLogfile( parser.value( logOption ) );
  if ( parser.isSet( bindAddrOption ) )
    dConf->setBindaddr( parser.value( bindAddrOption ) );
  if ( parser.isSet( portOption ) )
    dConf->setBindport( parser.value( portOption ) );
  //
  // erzeuge den Server mit der Config
  //
  bose_commserver::BoseCommServer server( dConf );
  QObject::connect( &server, &bose_commserver::BoseCommServer::closed, &app, &QCoreApplication::quit );
  //
  // und ab dafür...
  // exec waret auf quit
  //
  return app.exec();
}
