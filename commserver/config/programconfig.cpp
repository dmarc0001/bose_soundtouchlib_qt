#include "programconfig.hpp"

namespace bose_commserver
{
  const char *ProgramConfig::logfile{"commserver.log"};
  const char *ProgramConfig::bindaddr{"0.0.0.0"};
  const char *ProgramConfig::bindport{"8080"};
  const char *ProgramConfig::wsport{"8080"};
  const char *ProgramConfig::httpport{"8090"};
  const char *ProgramConfig::logger{"commserver.log"};
  const char *ProgramConfig::logpath{""};
  const LgThreshold ProgramConfig::threshold{LgThreshold::LG_CRIT};
}  // namespace bose_commserver
