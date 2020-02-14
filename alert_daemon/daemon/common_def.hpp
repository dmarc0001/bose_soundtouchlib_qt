#ifndef COMMON_DEF_HPP
#define COMMON_DEF_HPP

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include "config/alertconfig.hpp"

namespace bose_commserver
{
  //
  // Hilfestellung für einfacheres lesen
  //
  using JsonObjSPtr = std::shared_ptr< QJsonObject >;
  using JSonStringPtr = std::shared_ptr< QString >;
  using AppConfigPtr = std::shared_ptr< AlertAppConfig >;
  using LoggerPtr = std::shared_ptr< Logger >;

#ifdef DEBUG
  //
  // das Format "aufgehübscht" (0)
  //
  constexpr QJsonDocument::JsonFormat jsonFormat = QJsonDocument::Indented;
#else
  //
  // das Format platzsparend (1)
  //
  constexpr QJsonDocument::JsonFormat jsonFormat = QJsonDocument::Compact;
#endif

}  // namespace bose_commserver

#endif
