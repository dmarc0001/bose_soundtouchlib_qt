#include "xmlparser/httpresponse/httpsourcesobject.hpp"

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
    deviceId = IResponseObject::getAttribute( reader, QLatin1String( "deviceID" ) );
    qDebug() << "device id: " << deviceId;
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    while ( IResponseObject::getNextStartTag( reader ) )
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
    sourceItem.source = IResponseObject::getAttribute( reader, QLatin1String( "source" ) );
    qDebug() << "source: " << sourceItem.source;
    // sourceAccount
    sourceItem.sourceAccount = IResponseObject::getAttribute( reader, QLatin1String( "sourceAccount" ) );
    qDebug() << "source account: " << sourceItem.sourceAccount;
    // status
    sourceItem.status = IResponseObject::getAttribute( reader, QLatin1String( "status" ) );
    qDebug() << "source status: " << sourceItem.status;
    // isLocal
    if ( IResponseObject::getAttribute( reader, QLatin1String( "isLocal" ) ) == QLatin1String( "true" ) )
    {
      sourceItem.isLocal = true;
      qDebug() << "is local: " << sourceItem.isLocal;
    }
    // multiroomallowed
    if ( IResponseObject::getAttribute( reader, QLatin1String( "multiroomalowed" ) ) == QLatin1String( "true" ) )
    {
      sourceItem.multiroomallowed = true;
      qDebug() << "multi roo alowed: " << sourceItem.multiroomallowed;
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
