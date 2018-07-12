#include "httpresulterrorobject.hpp"

namespace bose_soundtoch_lib
{
  /**
   * @brief HttpResultErrorObject::HttpResultErrorObject
   * @param xmlreader
   * @param parent
   */
  HttpResultErrorObject::HttpResultErrorObject( QDomElement *domElem, QObject *parent ) : IResponseObject( domElem, parent )
  {
    Q_ASSERT( domElem->tagName() == QLatin1String( "errors" ) );
    resultType = ResultobjectType::R_ERROR;
    //
    // ID finden
    //
    deviceId = IResponseObject::getAttribute( domElem, QLatin1String( "deviceID" ) );
    qDebug() << "device id: " << deviceId;
    //
    // lese soweit neue Elemente vorhanden sind, bei schliessendem Tag -> Ende
    //
    QDomNodeList rootChildNodesList( domElem->childNodes() );
    for ( int nodeIdx = 0; nodeIdx < rootChildNodesList.length(); nodeIdx++ )
    {
      QDomNode currNode( rootChildNodesList.item( nodeIdx ) );
      if ( currNode.isNull() )
        continue;
      //
      // unterscheide die Knoten
      // der Name ist hier als QString
      //
      QString currName( currNode.nodeName() );
      if ( currName == QLatin1String( "error" ) )
      {
        //
        // einzelnen Fehler Parsen
        //
        parseError( &currNode );
      }
      else
      {
        //
        // unsupportet elements
        //
        qWarning() << "unsupported tag: " << currName << " -> " << currNode.toElement().text();
      }
    }
  }

  /**
   * @brief HttpResultErrorObject::~HttpResultErrorObject
   */
  HttpResultErrorObject::~HttpResultErrorObject()
  {
    qDebug() << "...";
  }

  /**
   * @brief HttpResultErrorObject::parseError
   */
  void HttpResultErrorObject::parseError( QDomNode *node )
  {
    Q_ASSERT( node->nodeName() == QLatin1String( "error" ) );
    //
    // einzelnen Fehler parsen und speichern
    //
    ResponseError error;
    qDebug() << "...";
    error.value = IResponseObject::getAttribute( node, QLatin1String( "value" ) ).toInt();
    qDebug() << "error value: " << error.value;
    error.name = IResponseObject::getAttribute( node, QLatin1String( "name" ) );
    qDebug() << "error name: " << error.name;
    error.severity = IResponseObject::getAttribute( node, QLatin1String( "severity" ) );
    qDebug() << " error severity: " << error.severity;
    //
    // den Fehlertext lesen
    //
    error.text = node->toElement().text();
  }

  QVector< ResponseError > HttpResultErrorObject::getErrors() const
  {
    return errors;
  }

}  // namespace bose_soundtoch_lib
