#ifndef BSOUNDTOUCH_GLOBAL_HPP
#define BSOUNDTOUCH_GLOBAL_HPP

#include <QPair>
#include <QString>
#include <QVector>

namespace radio
{
  //
  // Definitionen um die Benennung der Parameter lesbarer zu machen
  // Parameter "IPADDR", "MACADDR/DEVICEID"
  //
  //! Paare Soundtouch <"IPADDR", "MACADDR/DEVICEID">
  using SoundTouchMemberObject = QPair< QString, QString >;
  //! Liste mit Paaren
  using SoundTouchMemberList = QVector< SoundTouchMemberObject >;

}  // namespace radio
#endif  // BSOUNDTOUCH_GLOBAL_HPP
