// Linux systems will reset the signal after it gets raised
// According to the man page, we can get around this by using different include
#include <stdio.h>

// #include <bsd/signal.h>    doesn't seem to compile though
//#include <bsd/signal.h>
#include <csignal>

#include <sys/time.h>
#include "extern.h"
#include "person.h"

void checkpointing(int);
void shutdownRequest(int);
void shutdownAndPurgeRequest(int);
void purgeRequest(int);
void hardShutdownRequest(int);
void alrmsig(int);
void profsig(int);
extern void genericPurgeLdead(TBeing *ch);

void signalSetup(void)
{
  vlogf(LOG_MISC, "trapping signals");

  signal(SIGUSR1, purgeRequest);
  signal(SIGUSR2, shutdownAndPurgeRequest);
  signal(SIGQUIT, shutdownRequest);
  signal(SIGHUP, hardShutdownRequest);
  signal(SIGPIPE, SIG_IGN);
  signal(SIGINT, hardShutdownRequest);
  signal(SIGALRM, alrmsig);
  signal(SIGTERM, hardShutdownRequest);

  // This stuff crashes on the Solaris machine
  // set up the deadlock-protection
  struct itimerval itime;
  struct timeval interval;
  interval.tv_sec = 1200;
  interval.tv_usec = 0;
  itime.it_interval = interval;
  itime.it_value = interval;
  setitimer(ITIMER_VIRTUAL, &itime, 0);
  signal(SIGVTALRM, checkpointing);
}

void checkpointing(int)
{
  mud_assert(tics, "CHECKPOINT shutdown: tics not updated. (%d)", tics);
  tics = 0;
}

void shutdownAndPurgeRequest(int num)
{
  vlogf(LOG_MISC, "Received signal request to purge linkdeads and shut down");
  purgeRequest(num);
  shutdownRequest(num);
}

void purgeRequest(int)
{
  vlogf(LOG_MISC, "Received signal request to purge linkdeads");

  genericPurgeLdead(NULL);
}

void shutdownRequest(int)
{
  vlogf(LOG_MISC, "Received signal request to shut down");
  char buf[2000];

  int num = 5;
  if (!timeTill)
    timeTill = time(0) + (num * SECS_PER_REAL_MIN);
  else if (timeTill < (time(0) + (num * SECS_PER_REAL_MIN))) {
    vlogf(LOG_MISC, "Shutdown in progress overrides request.");
    return;
  } else {
    timeTill = time(0) + (num * SECS_PER_REAL_MIN);
  }

  sprintf(buf, "<r>******* SYSTEM MESSAGE ******<z>\n\r<c>%s in %ld minute%s.<z>\n\r",
     shutdown_or_reboot().c_str(),
     ((timeTill - time(0)) / 60), (((timeTill - time(0)) / 60) == 1) ? "" : "s");
  descriptor_list->worldSend(buf, NULL);
}

void hardShutdownRequest(int)
{
  vlogf(LOG_MISC, "Received signal request for hard shutdown");
  exit(0); /* something more elegant should perhaps be substituted */
}

void alrmsig(int)
{
  vlogf(LOG_MISC, "SIGALRM received. Ignoring.");
}

void profsig(int)
{
  // prof signals come in if prof/gprof is enabled
  // we have to process these sigs, but ignore them
  vlogf(LOG_MISC, "SIGPROF received. Ignoring.");
}
