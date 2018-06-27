#include "httpsourcesobject.hpp"

namespace bose_soundtoch_lib
{
  HttpSourcesObject::HttpSourcesObject( QXmlStreamReader *xmlreader, QObject *parent ) : IResponseObject( xmlreader, parent )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "sources" ) );
    resultType = ResultobjectType::R_SOURCES;
    //
    // Device ID finden (Attribute von <info>)
    //
    qDebug() << "...";
    deviceId = getAttibute( reader, QLatin1String( "deviceID" ) );
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
        qWarning() << "unsupported tag: " << reader->name().toString() << " --> " << reader->readElementText();
      }
    }
  }

  HttpSourcesObject::~HttpSourcesObject()
  {
    qDebug() << "...";
  }

  void HttpSourcesObject::parseSourceItem( void )
  {
    Q_ASSERT( reader->isStartElement() && reader->name() == QLatin1String( "sourceItem" ) );
    SourceItem sourceItem;
    //
    // source infos finden (Attribute von <sourceItem>)
    //
    qDebug() << "...";
    sourceItem.source = getAttibute( reader, QLatin1String( "source" ) );
    // sourceAccount
    sourceItem.sourceAccount = getAttibute( reader, QLatin1String( "sourceAccount" ) );
    // status
    sourceItem.status = getAttibute( reader, QLatin1String( "status" ) );
    // isLocal
    if ( getAttibute( reader, QLatin1String( "isLocal" ) ) == QLatin1String( "true" ) )
    {
      sourceItem.isLocal = true;
    }
    // multiroomallowed
    if ( getAttibute( reader, QLatin1String( "multiroomalowed" ) ) == QLatin1String( "true" ) )
    {
      sourceItem.multiroomallowed = true;
    }
    sourceItem.Content = reader->readElementText();
    qDebug() << "entry \"Content\" has value " << sourceItem.Content;
    //
    // in die Liste aufnehmen
    //
    sourceItems.append( sourceItem );
  }

  QVector< SourceItem > HttpSourcesObject::getSourceItems() const
  {
    return sourceItems;
  }

}  // namespace bose_soundtoch_lib
