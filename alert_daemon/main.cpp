#include "main.hpp"

using namespace bose_commserver;

int main( int argc, char *argv[] )
{
  QCoreApplication app( argc, argv );
  QCommandLineParser parser;
  std::shared_ptr< AlertConfig > dConf = std::shared_ptr< AlertConfig >( new AlertConfig() );
  QString appName( QCoreApplication::applicationName() );
  QString defaultini = appName.append( ".ini" );
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
  // config option
  //
  QCommandLineOption configOption( "config", QString( "special config file [default: %1]" ).arg( defaultini ), "config", defaultini );
  //
  // Logger Option, kein default
  //
  QCommandLineOption logOption( "logfile", QString( "logging file [default: %1]" ).arg( AlertConfig::getDefaultLogFile() ),
                                "filename" );
  //
  // bindaddr fuer dieseen Daemon
  //
  QCommandLineOption bindAddrOption( "bind", QString( "bind addr [default: %1]" ).arg( AlertConfig::getDefaultBindAddr() ),
                                     "local addr", AlertConfig::getDefaultBindAddr() );
  //
  // bindport fuer diesen daemon
  //
  QCommandLineOption portOption( "port", QString( "bind port [default: %1]" ).arg( AlertConfig::getDefaultBindPort() ), "local addr",
                                 AlertConfig::getDefaultBindPort() );
  parser.addOption( dbgOption );
  parser.addOption( configOption );
  parser.addOption( logOption );
  parser.addOption( bindAddrOption );
  parser.addOption( portOption );
  //
  // Auswerten der Optionen
  //
  parser.process( app );
  //
  // wenn eine andere config datei ausgewählt wurde
  // dann einstellungen aus dieser wählen
  //
  if ( parser.isSet( configOption ) )
  {
    dConf->loadSettings( parser.value( configOption ) );
  }
  else
  {
    dConf->loadSettings();
  }
  //
  dConf->setIsDebug( parser.isSet( dbgOption ) );
  // logoption überschreiben?
  if ( parser.isSet( logOption ) )
    dConf->setLogfileName( parser.value( logOption ) );
  // Bind addr überschreiben?
  if ( parser.isSet( bindAddrOption ) )
    dConf->setBindaddr( parser.value( bindAddrOption ) );
  // port option überschreiben?
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
