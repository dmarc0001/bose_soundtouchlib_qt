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
  try
  {
    bose_commserver::BoseCommServer server( dConf );
    QObject::connect( &server, &bose_commserver::BoseCommServer::closed, &app, &QCoreApplication::quit );
    //
    // Signalhandling einschalten
    //
    previousINTHandler = signal( SIGINT, signalHandler );
    previousTERMHandler = signal( SIGTERM, signalHandler );
    //
    // Server instanz vermerken
    //
    commInstance = &server;
    //
    // und ab dafür...
    // exec waret auf quit
    //
    return app.exec();
  }
  catch ( NoLoggerException &ex )
  {
    std::cerr << "ERROR: " << ex.what() << std::endl;
    exit( -1 );
  }
}

/**
 * @brief signalHandler
 * @param signal
 */
void signalHandler( int signal )
{
  if ( commInstance )
  {
    if ( signal == SIGINT )
    {
      // signal beackern
      commInstance->reciveSignal( signal );
    }
    else if ( signal == SIGTERM )
    {
      // signal beackern
      commInstance->reciveSignal( signal );
    }
#ifdef UNIX
    else if ( signal == SIGHUP )
    {
      // signal beackern
      commInstance->reciveSignal( signal );
    }
#endif
  }
}
