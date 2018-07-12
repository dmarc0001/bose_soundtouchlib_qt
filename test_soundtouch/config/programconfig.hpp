#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace bose_soundtoch_lib
{
  class ProgramConfig
  {
    public:
    const static char *logfile;
    const static char *server;
    const static char *wsport;
    const static char *httpport;
  };
}  // namespace radio
#endif  // CONFIG_HPP
