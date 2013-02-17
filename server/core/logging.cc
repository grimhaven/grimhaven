#include <execinfo.h>
#include <cstdio>
#include <map>

#include "core/logging.h"

// number of levels to print in a backtrace
const int MAX_TRACE_LEVELS = 10;

const std::map<LogSeverity, const sstring> severity_descriptions = {
  { LOG_LOW,     "L.O.W. Error" },
  { LOG_MISC,    "Misc" },
  { LOG_FILE,    "File I/O" },
  { LOG_BUG,     "BUG" },
  { LOG_PROC,    "Proc" },
  { LOG_PIO,     "Player I/O" },
  { LOG_IIO,     "Imm I/O" },
  { LOG_CLIENT,  "Client" },
  { LOG_COMBAT,  "Combat" },
  { LOG_FACT,    "Faction" },
  { LOG_MOB,     "Mob" },
  { LOG_MOB_AI,  "Mob AI" },
  { LOG_MOB_RS,  "Mob Response" },
  { LOG_OBJ,     "Object" },
  { LOG_EDIT,    "Editor" },
  { LOG_CHEAT,   "Cheating" },
  { LOG_DB,      "Database" },
  { LOG_BATOPR,  "Batopr" },
  { LOG_BRUTIUS, "Brutius" },
  { LOG_COSMO,   "Cosmo" },
  { LOG_LAPSOS,  "Lapsos" },
  { LOG_PEEL,    "Peel" },
  { LOG_JESUS,   "Jesus" },
  { LOG_DASH,    "Dash" },
  { LOG_ANGUS,   "Angus" },
  { LOG_MAROR,   "Maror" }
};

void vlogf_trace(LogSeverity severity, const sstring &message) {
  void *trace[MAX_TRACE_LEVELS];
  size_t count = backtrace(trace, MAX_TRACE_LEVELS);
  char **symbols = backtrace_symbols(trace, count);

  vlogf(severity, message);
  for (size_t i=0; i<count; i++)
    vlogf(severity, format("    at function: %s") % symbols[i]);

  free(symbols);
}

void vlogf(LogSeverity severity, const sstring &message) {
  sstring buf = message;
  time_t now = time(0);
  struct tm *local = localtime(&now);
  if (severity_descriptions.count(severity))
    buf = format("%s: %s") % severity_descriptions.at(severity) % buf;

  fprintf(stderr, "%4.4d|%2.2d%2.2d|%2.2d:%2.2d:%2.2d :: %s\n",
          local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
          local->tm_hour, local->tm_min, local->tm_sec, buf.c_str());
}
