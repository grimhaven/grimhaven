#ifndef SERVER_SPEC_OBJS_LOTTERY_TICKET_H_
#define SERVER_SPEC_OBJS_LOTTERY_TICKET_H_

#include "util/sstring.h"

const int NUM_LOTTERY_PRIZES=10;

const int OBJ_LOTTERY_TICKET = 2372;

struct LotteryPrizes {
  const sstring name;
  int vnum;
  int odds;
};

extern LotteryPrizes prizes[NUM_LOTTERY_PRIZES];

#endif
