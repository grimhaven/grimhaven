#ifndef __CMD_DISSECT_H
#define __CMD_DISSECT_H

#include <map>

class dissectInfo
{
  public:
    unsigned int loadItem;
    unsigned int amount;
    unsigned int count;
    sstring message_to_self;
    sstring message_to_others;

    dissectInfo *tNext;

    dissectInfo() :
      loadItem(0),
      amount(0),
      count(0),
      message_to_self(),
      message_to_others(),
      tNext(NULL)
    {
    }
};

extern std::map<unsigned short int, dissectInfo>dissect_array;

#endif
