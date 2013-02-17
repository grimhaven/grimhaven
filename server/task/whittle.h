#ifndef SERVER_TASK_WHITTLE_H_
#define SERVER_TASK_WHITTLE_H_

enum whittleTypeT
{
  WHITTLE_ERROR,
  WHITTLE_GENERAL,
  WHITTLE_EASY,
  WHITTLE_MEDIUM,
  WHITTLE_STANDARD,
  WHITTLE_HARD,
  WHITTLE_TOUGH,
  WHITTLE_DELICATE,
  WHITTLE_INVOLVED,
  WHITTLE_STRONG,
  WHITTLE_TIMECONSUMING,
  WHITTLE_VALUABLE,
  WHITTLE_MAX
};

enum whittlePulseT
{
  WHITTLE_PULSE_ZEROOUT,
  WHITTLE_PULSE_CARVED,
  WHITTLE_PULSE_SCRAPED,
  WHITTLE_PULSE_SMOOTHED,
  WHITTLE_PULSE_MAX
};

class taskWhittleEntry
{
  public:
    sstring        name;
    double        volSize,
                  weiSize,
                  weaSize;
    int           whittleReq,
                  itemVnum,
                  tClass;
    whittleTypeT  itemType;
    bool          valid,
                  affectValue;

    bool   operator==(sstring);
    sstring getName(bool);
    void   operator()(sstring, int, int, int, bool, whittleTypeT);

    taskWhittleEntry();
    ~taskWhittleEntry();
};

#endif
