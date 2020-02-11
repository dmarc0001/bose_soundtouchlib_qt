#ifndef ALERTCONFIG_HPP
#define ALERTCONFIG_HPP

#include <QObject>

class AlertConfig : public QObject
{
  Q_OBJECT
  public:
  explicit AlertConfig(QObject *parent = nullptr);

  signals:

};

#endif // ALERTCONFIG_HPP
