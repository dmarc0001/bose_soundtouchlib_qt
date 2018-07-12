#include "xmlresultparser.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief XmlResultParser::XmlResultParser
   * @param xmlString
   * @param parent
   */
  XmlResultParser::XmlResultParser( QString &xmlString, QObject *parent )
      : QObject( parent ), responseObject( nullptr ), wasError( false ), errMsg( "" ), errLine( 0 ), errColumn( 0 )
  {
    qDebug() << "...";
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
   * @brief XmlResultParser::~XmlResultParser
   */
  XmlResultParser::~XmlResultParser()
  {
    qDebug() << "...";
  }

  /**
   * @brief XmlResultParser::parseFile
   * @return
   */
  void XmlResultParser::parseDom( void )
  {
    QString rootelemName = docElem.tagName();
    //
    // zuordnung machen, IResponseObject ist polymorph, das erleichtert die Handhabung
    //
    if ( rootelemName == QLatin1String( "info" ) )
    {
      //
      // Device INFO erhalten
      // erzeuge das Objekt und Parse es
      //
      responseObject = std::shared_ptr< IResponseObject >( new HttpDeviceInfoObject( &docElem, this ) );
    }
    else if ( rootelemName == QLatin1String( "nowPlaying" ) )
    {
      //
      // now playing erhalten
      // erzeuge das Objekt und Parse es
      //
      responseObject = std::shared_ptr< IResponseObject >( new HttpNowPlayingObject( &docElem, this ) );
    }
    else if ( rootelemName == QLatin1String( "volume" ) )
    {
      //
      // volume erhalten
      // erzeuge das Objekt und Parse es
      //
      responseObject = std::shared_ptr< IResponseObject >( new HttpVolumeObject( &docElem, this ) );
    }
    else if ( rootelemName == QLatin1String( "sources" ) )
    {
      //
      // sources erhalten
      // erzeuge das Objekt und Parse es
      //
      responseObject = std::shared_ptr< IResponseObject >( new HttpSourcesObject( &docElem, this ) );
    }
    else if ( rootelemName == QLatin1String( "zone" ) )
    {
      //
      // zoneninfo erhalten
      // erzeuge das Objekt und Parse es
      //
      responseObject = std::shared_ptr< IResponseObject >( new HttpZoneObject( &docElem, this ) );
    }
    else if ( rootelemName == QLatin1String( "bassCapabilities" ) )
    {
      //
      // bass möglichkeiten erhalten
      // erzeuge das Objekt und Parse es
      //
      responseObject = std::shared_ptr< IResponseObject >( new HttpBassCapabilitiesObject( &docElem, this ) );
    }
    else if ( rootelemName == QLatin1String( "bass" ) )
    {
      //
      // bass einstellungen erhalten
      // erzeuge das Objekt und Parse es
      //
      responseObject = std::shared_ptr< IResponseObject >( new HttpBassObject( &docElem, this ) );
    }
    else if ( rootelemName == QLatin1String( "presets" ) )
    {
      //
      // preset einstellungen erhalten
      // erzeuge das Objekt und Parse es
      //
      responseObject = std::shared_ptr< IResponseObject >( new HttpPresetsObject( &docElem, this ) );
    }
    else if ( rootelemName == QLatin1String( "group" ) )
    {
      //
      // Gruppeneinstellungen (nur SoundTouch10)
      // erzeuge das Objekt und Parse es
      //
      responseObject = std::shared_ptr< IResponseObject >( new HttpGroupObject( &docElem, this ) );
    }
    else if ( rootelemName == QLatin1String( "status" ) )
    {
      //
      // alles OK bei "set" Funktion
      // erzeuge das Objekt und Parse es
      //
      responseObject = std::shared_ptr< IResponseObject >( new HttpResultOkObject( &docElem, this ) );
    }
    else if ( rootelemName == QLatin1String( "errors" ) )
    {
      //
      // FEHLER bei "set" Funktion
      // erzeuge das Objekt und Parse es
      //
      responseObject = std::shared_ptr< IResponseObject >( new HttpResultErrorObject( &docElem, this ) );
    }
    else
    {
      //
      // hier weiss ich auch nicht weiter, der TAG ist unbekannt
      //
      qWarning() << "unsupported tag in response XML struct found: " << docElem.tagName();
    }
  }

  //
  // GETTER
  //

  bool XmlResultParser::hasError( void )
  {
    return ( wasError );
  }

  QString XmlResultParser::getErrorString( void )
  {
    if ( wasError )
    {
      QString errStr( QString( "error %1 at line %2, col %2" ).arg( errMsg ).arg( errLine ).arg( errColumn ) );
      return ( errStr );
    }
    return ( QLatin1String( "no error" ) );
  }

  std::shared_ptr< IResponseObject > XmlResultParser::getResultObject( void )
  {
    return ( responseObject );
  }

}  // namespace bose_soundtoch_lib
