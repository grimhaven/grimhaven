//////////////////////////////////////////////////////////////////////////
//
// SneezyMUD - All rights reserved, SneezyMUD Coding Team
//
//////////////////////////////////////////////////////////////////////////

#include "socket.h"
#include "database.h"
#include "configuration.h"
#include "extern.h"
#include "enum.h"

#include <stdio.h>

extern "C" {
#include <unistd.h>
}

extern int run_the_game();


#ifndef LOWTOOLS

int main(int argc, char *argv[])
{
  int a;

  if (!Config::doConfiguration(argc, argv))
    exit(1);

  if (chdir(Config::DataDir().c_str()) < 0) {
    std::cout << format("Failed to chdir to lib directory '%s'\n") % Config::DataDir();
    perror("chdir");
    exit(1);
  }

  vlogf(LOG_MISC, format("Using %s as data directory.") % Config::DataDir());

  if (Config::NoSpecials())
    vlogf(LOG_MISC, "Suppressing assignment of special routines.");

  Uptime = time(0);
  srand(Uptime);

  vlogf(LOG_MISC, format("Running %s on port %d.") %  MUD_NAME_VERS % gamePort);

  WizLock = Config::WizLock();
  if (WizLock)
    vlogf(LOG_MISC, "Starting with wizlock enabled");

  vlogf(LOG_MISC, "Blanking denied hosts.");
  for (a = 0; a < MAX_BAN_HOSTS; a++) {
    strcpy(hostLogList[a], "");
    strcpy(hostlist[a], "");
  }
  numberhosts = 0;
  numberLogHosts = 0;

#if 0
  // graceful, but too bad its not informative about the exception
  // (could try vlogf_trace here instead of assert?)
  try {
    run_the_game();
  } catch (...) {
    mud_assert(0, "Caught an exception");
  }
#else
  run_the_game();
#endif

  generic_cleanup();

  return (0);
}

#endif
