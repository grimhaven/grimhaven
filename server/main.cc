#include <errno.h>

#include "sys/comm.h"
#include "sys/configuration.h"
#include "sys/socket.h"
#include "misc/enum.h"
#include "misc/extern.h"


int game_main(int argc, char *argv[])
{
  if (!Config::doConfiguration(argc, argv)) {
    vlogf(LOG_FILE, "failed configuration");
    exit(1);
  }

  if (!chdir(Config::DataDir().c_str())) {
    vlogf(LOG_FILE, format("failed chdir to lib directory '%s': %s") %
            Config::DataDir() % strerror(errno));
    exit(1);
  }

  vlogf(LOG_MISC, format("Using %s as data directory.") % Config::DataDir());

  if (Config::NoSpecials())
    vlogf(LOG_MISC, "Suppressing assignment of special routines.");

  if (WizLock)
    vlogf(LOG_MISC, "Starting with wizlock enabled");

  Uptime = time(0);
  srand(Uptime);

  run_the_game();

  generic_cleanup();

  return 0;
}


int main(int argc, char **argv)
{
  try {
    return game_main(argc, argv);
  } catch (const std::exception &e) {
    vlogf_trace(LOG_BUG, format("Exception (std::exception) thrown: %s") % e.what() );
  } catch (const char *s) {
    vlogf_trace(LOG_BUG, format("Exception (char *) thrown: %s") % s );
  } catch (const std::string &s) {
    vlogf_trace(LOG_BUG, format("Exception (std::string) thrown: %s") % s );
  }
  return -1;
}
