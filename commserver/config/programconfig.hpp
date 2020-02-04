#ifndef CONFIG_HPP
#define CONFIG_HPP

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
  };
}  // namespace bose_commserver
#endif  // CONFIG_HPP
