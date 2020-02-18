#include "main.hpp"

using namespace bose_commserver;

int main( int argc, char *argv[] )
{
  QCoreApplication app( argc, argv );
  QCommandLineParser parser;
  std::shared_ptr< AlertAppConfig > dConf = std::shared_ptr< AlertAppConfig >( new AlertAppConfig() );
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
  QCommandLineOption logOption( "logfile", QString( "logging file [default: %1]" ).arg( AlertAppConfig::getDefaultLogFile() ),
                                "filename" );
  //
  // bindaddr fuer dieseen Daemon
  //
  QCommandLineOption bindAddrOption( "bind", QString( "bind addr [default: %1]" ).arg( AlertAppConfig::getDefaultBindAddr() ),
                                     "local addr", AlertAppConfig::getDefaultBindAddr() );
  //
  // bindport fuer diesen daemon
  //
  QCommandLineOption portOption( "port", QString( "bind port [default: %1]" ).arg( AlertAppConfig::getDefaultBindPort() ),
                                 "local addr", AlertAppConfig::getDefaultBindPort() );
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
  // SIGNAL HANDLER INITIIEREN
  setup_unix_signal_handlers();
  //
  // und ab dafür...
  // exec waret auf quit
  //
  return app.exec();
}

/**
 * @brief setup_unix_signal_handlers
 * @return
 */
static int setup_unix_signal_handlers()
{
  struct sigaction /*hup,*/ term;

  /*
  hup.sa_handler = MyDaemon::hupSignalHandler;
  sigemptyset(&hup.sa_mask);
  hup.sa_flags = 0;
  hup.sa_flags |= SA_RESTART;

  if (sigaction(SIGHUP, &hup, 0))
    return 1;
  */

  term.sa_handler = bose_commserver::BoseCommServer::termSignalHandler;
  sigemptyset( &term.sa_mask );
  term.sa_flags |= SA_RESTART;

  if ( sigaction( SIGTERM, &term, 0 ) )
    return 2;

  return 0;
}
