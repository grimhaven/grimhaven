#ifndef __DB_H
#define __DB_H

#include "structs.h"
#include "sstring.h"
#include "mudinfo.h"

#include <map>

#define WORLD_SIZE        50000
#define ZONE_ROOM_RANDOM  -99
#define MAX_OBJ_AFFECT    5

extern bool bootTime;

namespace File {
  extern const char * const CREDITS;
  extern const char * const HELP_PAGE;
  extern const char * const MOTD;
  extern const char * const NEWS;
  extern const char * const SOCMESS;
  extern const char * const STATS;
  extern const char * const STATS_BAK;
  extern const char * const TIPS;
  extern const char * const VERSION;
  extern const char * const WIZLIST;
  extern const char * const WIZNEWS;
  extern const char * const WIZMOTD;

  extern const char * const ANSI_MENU_1;
  extern const char * const ANSI_MENU_2;
  extern const char * const ANSI_MENU_3;
  extern const char * const ANSI_OPEN;
  extern const char * const NORM_MENU_1;
  extern const char * const NORM_MENU_2;
  extern const char * const NORM_MENU_3;
  extern const char * const NORM_OPEN;

  extern const char * const FACTIONS;
  extern const char * const FACTIONS_BAK;
  extern const char * const GUILDS;
  extern const char * const GUILDS_BAK;
}

namespace Path {
  extern const char * const HELP;
  extern const char * const IMMORTAL_HELP;
  extern const char * const BUILDER_HELP;
  extern const char * const SKILL_HELP;
  extern const char * const SPELL_HELP;
}

/* public procedures in db.c */

void bootDb(void);
int create_entry(char *name);
void zone_update(void);
int real_object(int);
int real_mobile(int);

// forward class decl
class resetCom;
class armorSetLoad;
class zoneData;
class extraDescription;
class indexData;
class objIndexData;
class mobIndexData;
class resetQElement;

enum readFileTypeT {
     REAL,
     VIRTUAL
};

typedef unsigned int resetFlag;
const resetFlag resetFlagNone = 0;
const resetFlag resetFlagBootTime = 1<<0;
const resetFlag resetFlagFindLoadPotential = 1<<1;
const resetFlag resetFlagPropLoad = 1<<2;

const resetFlag resetFlagCount = 3;
const resetFlag resetFlagMax = 1<<resetFlagCount;

class resetCom {
  public:
    char command;
    int if_flag;
    int arg1;
    int arg2;
    int arg3;
    int arg4;
    char character;
    int cmd_no;

  public:
    resetCom();
    resetCom(const resetCom &t);
    ~resetCom();
    resetCom & operator =(const resetCom &t);

    bool hasLoadPotential();
    bool usesRandomRoom();
    bool shouldStickToMob(bool &lastComStuck);

    // returns false to stop execution (critical fail or stop command)
    bool execute(zoneData &zone, resetFlag flags, bool &mobload, TMonster *&mob, bool &objload, TObj *&obj, bool &last_cmd);

  private:

    enum resetCommandId
    {
      cmd_Stop = 0, // S
      cmd_LoadMob, // M
      cmd_LoadMobGrouped, // K
      cmd_LoadMobCharmed, // C
      cmd_LoadMobRidden, // R
      cmd_SetRandomRoom, // A
      cmd_LoadChance, // ?
      cmd_LoadObjGround, // B
      cmd_LoadObjGroundBoot, // O
      cmd_LoadObjPlaced, // P
      cmd_LoadObjInventory, // G
      cmd_LoadObjEquipped, // E
      cmd_CreateLocalSet, // X
      cmd_LoadObjSetLocal, // Z
      cmd_LoadObjSet, // Y
      cmd_ChangeFourValues, // V
      cmd_SetTrap, // T
      cmd_SetHate, // H
      cmd_SetFear, // F
      cmd_SetDoor, // D
      cmd_LoadLoot, // L
      cmd_LoadObjEquippedProp, // I
      cmd_LoadObjSetLocalProp, // J

      cmd_Max
    };
    typedef void exec_fn(zoneData &, resetCom &, resetFlag, bool &, TMonster *&, bool &, TObj *&, bool &);
    static exec_fn *executeMethods[cmd_Max];

    resetCommandId getCommandId();
};

class armorSetLoad
{
private:
  struct armor_set_struct {
    int slots[24]; // should be MAX_WEAR
  } local_armor[16];
public:
  armorSetLoad();
  void resetArmor();
  void setArmor(int set, int slot, int value);
  int getArmor(int set, int slot);
};


class zoneData
{
  public:
    char *name;             // name of this zone
    int zone_nr;            // number of this zone
    int lifespan;           // how long between resets (minutes)
    int age;                // current age of this zone (minutes)
    int bottom;
    int top;                // upper limit for rooms in this zone
    int reset_mode;         // conditions for reset (see below)
    bool enabled;           // whether zone is enabled
    byte zone_value;
    unsigned int num_mobs;
    double mob_levels;
    double min_mob_level;
    double max_mob_level;
    int random_room;

    // the following stat_* variables are intended to be used for zone reporting in the stat zone command
    // they are not air-tight counts and should not be treated as such
    std::map<int, int> stat_mobs; // key: real mob number, value: count of that mob loading in the zonefile
    std::map<int, int> stat_objs; // key: real obj number, value: count of that obj loading in the zonefile
    // note the count value for stat_objs ignores things like load rates so is pretty useless info
    // it also doesn't contain global suitset objs and doesn't check to see if local suitsets actually load, so...

    int stat_mobs_total;     // total # of mobs loading in the zonefile
    int stat_mobs_unique;    // unique # of mobs loading in the zonefile
    int stat_objs_unique;    // unique # of objects loading in the zonefile

    armorSetLoad armorSets;

    bool isEmpty(void);
    void resetZone(bool bootTime, bool findLoadPotential=false);
    void closeDoors(void);
    void logError(char, const char *, int, int);
    void nukeMobs(void);
    void sendTo(sstring, int exclude_room=-1);
    bool doGenericReset(void);
    bool bootZone(int);
    void renumCmd(void);

    std::vector<resetCom>cmd;          // command table for reset

    zoneData();
    zoneData(const zoneData &t);
    ~zoneData();
    zoneData & operator= (const zoneData &t);
};


class indexData {
 public:
  int virt;
  long pos;
 private:
  int number;
  int max_num;
 public:
  const char *name;
  const char *short_desc;
  const char *long_desc;
  const char *description;

  short max_exist;        // for objs and mobs
  int spec;
  float weight;

  void addToNumber(const short int n){
    //    vlogf(LOG_PEEL, fmt("adding %i to number %i for object %i") %
    //          n % number % virt);
    number+=n;
  }

  void setMaxNumber(const short int n){
    max_num=n;
  }

  int getNumber(){
    return number;
  }

  int getMaxNumber(){
    return max_num;
  }

  indexData();
  indexData(const indexData &);
  indexData & operator= (const indexData &a);
  virtual ~indexData();
};

class objIndexData : public indexData
{
  public:
    extraDescription *ex_description;  // extra descriptions
    objAffData affected[MAX_OBJ_AFFECT];
    byte max_struct;
    short armor;
    unsigned int where_worn;
    ubyte itemtype;
    int value;

    objIndexData();
    objIndexData(const objIndexData &);
    objIndexData & operator=(const objIndexData &);
    virtual ~objIndexData();
};

class mobIndexData : public indexData
{
  public:
    long faction;
    long Class;
    long level;
    long race;
    bool doesLoad;
    int numberLoad;

    mobIndexData();
    mobIndexData(const mobIndexData &);
    mobIndexData & operator=(const mobIndexData &);
    virtual ~mobIndexData();
};

class resetQElement
{
  public:
    unsigned int zone_to_reset;
    resetQElement *next;

  resetQElement() :
    zone_to_reset(0),
    next(NULL)
  {
  }
};

#endif
