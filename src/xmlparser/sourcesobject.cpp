#include "sourcesobject.hpp"

namespace radio
{
  SourcesObject::SourcesObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent )
      : IResponseObject( logger, xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "sources" ) );
    resultType = ResultobjectType::R_SOURCES;
    //
    // Device ID finden (Attribute von <info>)
    //
    lg->debug( "SourcesObject::SourcesObject..." );
    lg->debug( "SourcesObject::SourcesObject: check for attribute \"deviceID\"..." );
    QXmlStreamAttributes attr = reader->attributes();
    if ( attr.hasAttribute( QLatin1String( "deviceID" ) ) )
    {
      deviceId = attr.value( QLatin1String( "deviceID" ) ).toString();
      lg->debug( QString( "SourcesObject::SourcesObject: attribute \"deviceID\" has value %1" ).arg( deviceId ) );
    }
    else
    {
      lg->warn( "SourcesObject::SourcesObject: there is no attribute \"deviceID\"..." );
    }
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( reader->readNextStartElement() && !reader->hasError() )
    {
      //
      // das nächste element bearbeiten, welches ist es?
      //
      if ( reader->name() == QLatin1String( "sourceItem" ) )
      {
        //
        // Ein Eintrag über eine source
        //
        parseSourceItem();
      }
      else
      {
        //
        // unsupportet elements
        //
        lg->debug(
            QString( "SourcesObject::SourcesObject: %1 -> %2" ).arg( reader->name().toString() ).arg( reader->readElementText() ) );
      }
    }
  }

  SourcesObject::~SourcesObject()
  {
    lg->debug( "SourcesObject::~SourcesObject..." );
  }

  void SourcesObject::parseSourceItem( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "sourceItem" ) );
    SourceItem sourceItem;
    //
    // source infos finden (Attribute von <sourceItem>)
    //
    lg->debug( "SourcesObject::parseSourceItem..." );
    lg->debug( "SourcesObject::parseSourceItem: check for attribute in \"sourceItem\"..." );
    QXmlStreamAttributes attr = reader->attributes();
    // source
    if ( attr.hasAttribute( QLatin1String( "source" ) ) )
    {
      sourceItem.source = attr.value( QLatin1String( "source" ) ).toString();
      lg->debug( QString( "SourcesObject::parseSourceItem: attribute \"source\" has value %1" ).arg( sourceItem.source ) );
    }
    else
    {
      lg->warn( "SourcesObject::parseSourceItem: there is no attribute \"source\"..." );
    }
    // sourceAccount
    if ( attr.hasAttribute( QLatin1String( "sourceAccount" ) ) )
    {
      sourceItem.sourceAccount = attr.value( QLatin1String( "sourceAccount" ) ).toString();
      lg->debug(
          QString( "SourcesObject::parseSourceItem: attribute \"sourceAccount\" has value %1" ).arg( sourceItem.sourceAccount ) );
    }
    else
    {
      lg->warn( "SourcesObject::parseSourceItem: there is no attribute \"sourceAccount\"..." );
    }
    // status
    if ( attr.hasAttribute( QLatin1String( "status" ) ) )
    {
      sourceItem.status = attr.value( QLatin1String( "status" ) ).toString();
      lg->debug( QString( "SourcesObject::parseSourceItem: attribute \"status\" has value %1" ).arg( sourceItem.status ) );
    }
    else
    {
      lg->warn( "SourcesObject::parseSourceItem: there is no attribute \"status\"..." );
    }
    // isLocal
    if ( attr.hasAttribute( QLatin1String( "isLocal" ) ) )
    {
      if ( attr.value( QLatin1String( "isLocal" ) ) == QLatin1String( "true" ) )
      {
        sourceItem.isLocal = true;
      }
      lg->debug( QString( "SourcesObject::parseSourceItem: attribute \"isLocal\" has value %1" ).arg( sourceItem.isLocal ) );
    }
    // multiroomallowed
    if ( attr.hasAttribute( QLatin1String( "multiroomallowed" ) ) )
    {
      if ( attr.value( QLatin1String( "multiroomallowed" ) ) == QLatin1String( "true" ) )
      {
        sourceItem.multiroomallowed = true;
      }
      lg->debug( QString( "SourcesObject::parseSourceItem: attribute \"multiroomallowed\" has value %1" )
                     .arg( sourceItem.multiroomallowed ) );
    }
    else
    {
      lg->warn( "SourcesObject::parseSourceItem: there is no attribute \"status\"..." );
    }
    sourceItem.Content = reader->readElementText();
    lg->debug( QString( "SourcesObject::parseSourceItem: entry \"Content\" has value %1" ).arg( sourceItem.Content ) );
    //
    // in die Liste aufnehmen
    //
    sourceItems.append( sourceItem );
  }

}  // namespace radio
