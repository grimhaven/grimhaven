#ifndef SERVER_MUD_H_
#define SERVER_MUD_H_

#include <time.h>

#include "core/configuration.h"

namespace mud {

class Server {
 public:
  inline time_t started() { return started_; }
  inline time_t booted() { return booted_; }
  inline time_t shutdown() { return shutdown_; }
 private:
  // wall time when mud startup began
  time_t started_;
  // wall time when booting finished, zero if not booted yet
  time_t booted_;
  // wall time when shutdown should happen, zero if no shutdown in progress
  time_t shutdown_;

  Configuration config;
  Wizlock wizlock;
};

}

#endif

// vim: ft=cpp:tw=79:sw=2:sts=2:ts=8:et
