#include <errno.h>
#include <string.h>
#include <iostream>

#include "core/configuration.h"
#include "core/logging.h"
#include "util/sstring.h"
#include "misc/extern.h"

Configuration Config;

time_t Uptime;

int game_main(int argc, const char *argv[]) {
  sstring msg = Config.load(argc, argv);
  if (!msg.empty()) {
    std::cout << msg << std::endl;
    vlogf(LOG_FILE, "Failed to load configuration");
    exit(1);
  }

  if (chdir(Config.data_dir().c_str())) {
    vlogf(LOG_FILE, format("Failed chdir to data directory '%s': %s") %
            Config.data_dir() % strerror(errno));
    exit(1);
  }

  vlogf(LOG_MISC, format("Using '%s' as data directory.") % Config.data_dir());

  Uptime = time(0);
  srand(Uptime);

  run_the_game();

  generic_cleanup();

  return 0;
}

int main(int argc, const char **argv) {
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
