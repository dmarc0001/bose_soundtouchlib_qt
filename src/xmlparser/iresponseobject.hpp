#ifndef IRESPONSEOBJECT_HPP
#define IRESPONSEOBJECT_HPP

#include <qglobal.h>
#include <QObject>
#include <QXmlStreamReader>
#include <memory>
#include "../logging/Logger.hpp"

namespace radio
{
  enum class ResultobjectType : quint8
  {
    R_OK,
    R_ERROR,
    R_DEVICE_INFO,
    R_NOW_PLAYING,
    R_VOLUME,
    R_SOURCES,
    R_ZONES,
    R_BASS_CAPABILITIES,
    R_BASS,
    R_PRESETS,
    R_GROUPS,
    R_UNKNOWN,
  };

  class IResponseObject : public QObject
  {
    Q_OBJECT
    protected:
    std::shared_ptr< Logger > lg;
    QXmlStreamReader *reader;
    ResultobjectType resultType;

    public:
    explicit IResponseObject( std::shared_ptr< Logger > logger, QXmlStreamReader *xmlreader, QObject *parent );
    virtual ~IResponseObject();
    ResultobjectType getResultType( void );
  };
}  // namespace radio
#endif  // IRESPONSEOBJECT_HPP
