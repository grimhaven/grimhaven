#include <stdio.h>
#include <execinfo.h>

#include "logging.h"

// number of levels to print in a backtrace
#define MAX_TRACE_LEVELS 10

const sstring getLogType(const logTypeT err)
{
  sstring s;
  switch (err) {
    case LOG_LOW:     s = "L.O.W. Error"; break;
    case LOG_MISC:    s = "Misc"; break;
    case LOG_FILE:    s = "File I/O"; break;
    case LOG_BUG:     s = "BUG"; break;
    case LOG_PROC:    s = "Proc"; break;
    case LOG_PIO:     s = "Player I/O"; break;
    case LOG_IIO:     s = "Imm I/O"; break;
    case LOG_CLIENT:  s = "Client"; break;
    case LOG_COMBAT:  s = "Combat"; break;
    case LOG_FACT:    s = "Faction"; break;
    case LOG_MOB:     s = "Mob"; break;
    case LOG_MOB_AI:  s = "Mob AI"; break;
    case LOG_MOB_RS:  s = "Mob Response"; break;
    case LOG_OBJ:     s = "Object"; break;
    case LOG_EDIT:    s = "Editor"; break;
    case LOG_CHEAT:   s = "Cheating"; break;
    case LOG_DB:      s = "Database"; break;
    case LOG_BATOPR:  s = "Batopr"; break;
    case LOG_BRUTIUS: s = "Brutius"; break;
    case LOG_COSMO:   s = "Cosmo"; break;
    case LOG_LAPSOS:  s = "Lapsos"; break;
    case LOG_PEEL:    s = "Peel"; break;
    case LOG_JESUS:   s = "Jesus"; break;
    case LOG_DASH:    s = "Dash"; break;
    case LOG_ANGUS:   s = "Angus"; break;
    case LOG_MAROR:   s = "Maror"; break;
    default:          s= ""; break;
  }
  return s;
}

void vlogf_trace(const logTypeT err, const sstring &msg)
{
  void *trace[MAX_TRACE_LEVELS];
  size_t count = backtrace(trace, MAX_TRACE_LEVELS);
  char **symbols = backtrace_symbols(trace, count);

  vlogf(err, msg);
  for (size_t i=0; i<count; i++)
    vlogf(err, format("    at function: %s") % symbols[i]);

  free(symbols);
}

void vlogf(const logTypeT err, const sstring &msg)
{
  time_t lt = time(0);
  struct tm *now = localtime(&lt);
  sstring type = getLogType(err);
  sstring buf = type=="" ? msg : format("%s: %s") % type % msg;

  fprintf(stderr, "%4.4d|%2.2d%2.2d|%2.2d:%2.2d:%2.2d :: %s\n",
          now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
          now->tm_hour, now->tm_min, now->tm_sec, buf.c_str());
}

