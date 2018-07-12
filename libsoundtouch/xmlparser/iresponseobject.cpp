#include "iresponseobject.hpp"
#include <QDomNamedNodeMap>

namespace bose_soundtoch_lib
{
  // const QRegExp IResponseObject::shortTags( "<\\w+.*\\s+/>", Qt::CaseSensitive, QRegExp::RegExp );
  // const QRegExp IResponseObject::shortTags( "<(\\w+|\")(\\s+)?/>", Qt::CaseSensitive, QRegExp::RegExp );
  const QRegExp IResponseObject::shortTags( "<.*/>", Qt::CaseSensitive, QRegExp::RegExp );
  //
  //! für enum class ResultobjectType : quint8 aus "bsoundtouch_global.hpp" => value to String
  //
  const QMap< ResultobjectType, QString > IResponseObject::typeNames = {
      {ResultobjectType::R_OK, "OK"},
      {ResultobjectType::R_ERROR, "ERROR"},
      {ResultobjectType::R_DEVICE_INFO, "DEVICE INFO"},
      {ResultobjectType::R_NOW_PLAYING, "NOW PLAYING"},
      {ResultobjectType::R_VOLUME, "VOLUME"},
      {ResultobjectType::R_SOURCES, "SOURCES"},
      {ResultobjectType::R_ZONES, "ZONES"},
      {ResultobjectType::R_BASS_CAPABILITIES, "BASS CAPABILITIES"},
      {ResultobjectType::R_BASS, "BASS"},
      {ResultobjectType::R_PRESETS, "PRESETS"},
      {ResultobjectType::R_GROUPS, "GROUPS"},
      {ResultobjectType::U_SDKINFO, "UPDATE SDK INFO"},
      {ResultobjectType::U_PRESETS, "UPDATE PRESETS"},
      {ResultobjectType::U_NOWPLAYING, "UPDATE NOW PLAYING"},
      {ResultobjectType::U_SELECTION, "UPDATE SELECTION"},
      {ResultobjectType::U_VOLUME, "UPDATE VOLUME"},
      {ResultobjectType::U_BASS, "UPDATE BASS"},
      {ResultobjectType::U_ZONE, "UPDATE ZONE"},
      {ResultobjectType::U_INFO, "UPDATE DEVICE INFO"},
      {ResultobjectType::U_NAME, "UPDATE DEVICE NAME"},
      {ResultobjectType::U_ERROR, "UPDATE ERROR"},
      {ResultobjectType::U_GROUP, "UPDATE GROUP (ST10 only)"},
      {ResultobjectType::U_BROWSE_UNSUPPORTED, "UPDATE BROWSE (unsupported)"},
      {ResultobjectType::U_RECENTS_UNSUPPORTED, "UPDATE RECENTS (unsupported)"},
      {ResultobjectType::U_SOURCES_UNSUPPORTED, "UPDATE SOURCE (unsupported)"},
      {ResultobjectType::U_LANGUAGE_UNSUPPORTED, "UPDATE LANGUAGE (unsupported)"},
      {ResultobjectType::U_USER_ACTIVITY_UPDATED_UNSUPPORTED, "UPDATE USER ACTIVITY (unsupported)"},
      {ResultobjectType::U_USER_INACTIVITY_UPDATED_UNSUPPORTED, "UPDATE USER INACTIVITY (unsupported)"},
      {ResultobjectType::U_CONNECTION_STATE_UPDATED_UNSUPPORTED, "UPDATE CONNECTION STATE (unsupported)"},
      {ResultobjectType::U_AUDIOPRODUCT_TONECONTROLS_UNSUPPORTED, "UPDATE AUDIOPRODUCT TONE CONTROLS (unsupported)"},
      {ResultobjectType::U_AUDIOPRODUCT_LEVELCONTROLS_UNSUPPORTED, "UPDATE AUDIOPRODUCT LEVEL CONTROLS (unsupported)"},
      {ResultobjectType::U_AUDIO_SP_CONTROLS_UNSUPPORTED, "UPDATE AUDIO SP CONTROLS (unsupported)"},
      {ResultobjectType::R_UNKNOWN, "UNKNOWN"}};
  /**
   * @brief IResponseObject::IResponseObject
   * @param xmlreader
   * @param parent
   */
  IResponseObject::IResponseObject( QDomElement *_domElem, QObject *parent )
      : QObject( parent ), domElem( _domElem ), resultType( ResultobjectType::R_UNKNOWN )
  {
    deviceId.clear();
  }

  /**
   * @brief IResponseObject::~IResponseObject
   */
  IResponseObject::~IResponseObject()
  {
    //
    // hier ist erst einmal nichts aufzuräumen
    //
  }

  /**
   * @brief IResponseObject::setDeviceId
   * @param devId
   */
  void IResponseObject::setDeviceId( const QString &devId )
  {
    deviceId = QString( devId );
  }

  /**
   * @brief IResponseObject::getDeviceId
   * @return
   */
  QString IResponseObject::getDeviceId() const
  {
    return deviceId;
  }

  /**
   * @brief IResponseObject::getResultType
   * @return
   */
  ResultobjectType IResponseObject::getResultType( void )
  {
    return ( resultType );
  }

  /**
   * @brief IResponseObject::getResultTypeName
   * @return
   */
  QString IResponseObject::getResultTypeName( void ) const
  {
    return ( typeNames[ resultType ] );
  }

  /**
   * @brief IResponseObject::getAttibute (static) Attribute aus einem TAG lesen
   * @param reader
   * @param name
   * @return
   */
  QString IResponseObject::getAttribute( QDomNode *node, QLatin1String name )
  {
    //
    // attribute finden
    //
    QString result( QLatin1String( "UNKNOWN" ) );
    if ( !node->hasAttributes() )
    {
      //
      // Keine Attribute vorhanden
      //
      return ( result );
    }
    else
    {
      //
      // ist das gewünschte Attribut vorhanden?
      //
      QDomNamedNodeMap attribs = node->attributes();
      QDomNode attr( attribs.namedItem( name ) );
      if ( !attr.isNull() )
      {
        //
        // Ja, das Attribut ist:
        //
        result = attr.nodeValue();
      }
    }
    return ( result );
  }

  /**
   * @brief IResponseObject::getAttribute
   * @param domElement
   * @param name
   * @return
   */
  QString IResponseObject::getAttribute( QDomElement *domElement, QLatin1String name )
  {
    //
    // attribute finden
    //
    QString result( QLatin1String( "UNKNOWN" ) );
    if ( !domElement->hasAttribute( name ) )
    {
      //
      // Kein Attribut vorhanden
      //
      return ( result );
    }
    else
    {
      //
      // ist das gewünschte Attribut vorhanden?
      //
      QDomNamedNodeMap attribs = domElement->attributes();
      QDomNode attr( attribs.namedItem( name ) );
      if ( !attr.isNull() )
      {
        //
        // Ja, das Attribut ist:
        //
        result = attr.nodeValue();
      }
    }
    return ( result );
  }

}  // namespace bose_soundtoch_lib
