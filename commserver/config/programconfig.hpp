#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "logging/Logger.hpp"

namespace bose_commserver
{
  class ProgramConfig
  {
    public:
    const static char *logfile;
    const static char *bindaddr;
    const static char *bindport;
    const static char *wsport;
    const static char *httpport;
    const static char *logger;
    const static char *logpath;
    const static LgThreshold threshold;
  };
}  // namespace bose_commserver
#endif  // CONFIG_HPP
