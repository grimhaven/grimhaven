//////////////////////////////////////////////////////////////////////////
//
// SneezyMUD - All rights reserved, SneezyMUD Coding Team
//
// db.h , Database module.
// Usage: Loading/Saving chars booting world.
//
//////////////////////////////////////////////////////////////////////////




#ifndef __DB_H
#define __DB_H

#include "structs.h"
#include "sstring.h"

#include <map>

#define MUD_NAME          "Grimhaven"
#define MUD_VERS          "6.0"
#define MUD_NAME_VERS     MUD_NAME " v" MUD_VERS
#define WELCOME_MSG       "\n\rWelcome to " MUD_NAME_VERS "! " \
                          "May your journeys be enjoyable!\n\r\n\r"

#define MUD_DOMAIN        "grimhaven.org"
#define MUD_EMAIL         "mud@" MUD_DOMAIN
#define MUD_URL           "http://" MUD_DOMAIN
#define SOUNDS_URL        MUD_URL "/sounds/"
#define CLIENT_URL        MUD_URL "/client/"

#define WORLD_SIZE        50000
#define ZONE_ROOM_RANDOM  -99
#define MAX_OBJ_AFFECT    5

extern bool bootTime;

class File {
  private:
  File();
  
 public:
  static const char * const CREDITS;
  static const char * const HELP_PAGE;
  static const char * const MOTD;
  static const char * const NEWS;
  static const char * const SOCMESS;
  static const char * const STATS;
  static const char * const STATS_BAK;
  static const char * const TIPS;
  static const char * const VERSION;
  static const char * const WIZLIST;
  static const char * const WIZNEWS;
  static const char * const WIZMOTD;

  static const char * const ANSI_MENU_1;
  static const char * const ANSI_MENU_2;
  static const char * const ANSI_MENU_3;
  static const char * const ANSI_OPEN;
  static const char * const NORM_MENU_1;
  static const char * const NORM_MENU_2;
  static const char * const NORM_MENU_3;
  static const char * const NORM_OPEN;

  static const char * const FACTIONS;
  static const char * const FACTIONS_BAK;
  static const char * const GUILDS;
  static const char * const GUILDS_BAK;
};

class Path {
 private:
  Path();

 public:
  static const char * const HELP;
  static const char * const IMMORTAL_HELP;
  static const char * const BUILDER_HELP;
  static const char * const SKILL_HELP;
  static const char * const SPELL_HELP;
};  

const char * const MUDADMIN_EMAIL   ="mudadmin@sneezymud.com";
const char * const CODERS_EMAIL     ="mudadmin@sneezymud.com";

const int WORLD_SIZE = 50000;
const int ZONE_ROOM_RANDOM = -99;

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
    //	  n % number % virt);
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
