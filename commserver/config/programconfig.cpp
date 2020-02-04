#include "programconfig.hpp"

namespace bose_commserver
{
  const char *ProgramConfig::logfile{"commserver.log"};
  const char *ProgramConfig::bindaddr{"0.0.0.0"};
  const char *ProgramConfig::bindport{"8080"};
  const char *ProgramConfig::wsport{"8080"};
  const char *ProgramConfig::httpport{"8090"};
}  // namespace bose_commserver
