#ifndef SERVER_SYS_TIMING_H_
#define SERVER_SYS_TIMING_H_

#include<sys/time.h>


class TTiming {
  struct timeval tv_start, tv_end;
  struct timezone tz;
  double to_secs(struct timeval);

 public:
  void start();
  void end();
  double getStart();
  double getEnd();
  double getElapsed();
  double getElapsedReset();

  TTiming();
};



#endif
