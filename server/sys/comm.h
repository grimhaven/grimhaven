#ifndef SERVER_SYS_COMM_H_
#define SERVER_SYS_COMM_H_

#include "sys/ansi.h"
#include "util/sstring.h"
#include "misc/enum.h"

class TThing;
class TBeing;
class TRoom;
class TObj;
class sstring;

enum actToParmT {
     TO_ROOM,
     TO_VICT,
     TO_NOTVICT,
     TO_CHAR
};

void sendToAll(char *messg);
void sendToExcept(char *messg, TBeing *ch);
void sendToRoom(colorTypeT, const char *messg, int room);
void sendToRoom(const char *messg, int room);
void sendToRoomExcept(char *messg, int room, TBeing *ch);
void sendToRoomExceptTwo(char *, int, TBeing *, TBeing *);
void perform_to_all(char *messg, TBeing *ch);
void perform_complex(TBeing *, TBeing *, TObj *, TObj *, char *, char, int);
void sendrpf(int, colorTypeT, TRoom *, const char *,...);
void sendrpf(int, TRoom *, const char *,...);
void sendrpf(colorTypeT, TRoom *, const char *,...);
void sendrpf(TRoom *, const char *,...);
void sendToOutdoor(colorTypeT, const sstring &, const sstring &);
void colorAct(colorTypeT, const sstring &, bool, const TThing *, const TThing *, const TThing *, actToParmT, const char * color = NULL, int = 0);
void act(const sstring &, bool, const TThing *, const TThing *, const TThing *, actToParmT, const char * color = NULL, int = 0);
void nukeMobs(int);
bool isEmpty(int);

namespace Pulse {

const int COMMAND     =         0;
const int TICK        =         1;
const int ONE_SECOND  =         10;
const int EVERY       =         1;
const int MOBACT      =         (int)((float)ONE_SECOND * 1.2);
const int TELEPORT    =         (int)((float)ONE_SECOND * 1.2);
const int COMBAT      =         (int)((float)ONE_SECOND * 1.2);
const int DROWNING    =         (int)((float)ONE_SECOND * 3.6);
const int SPEC_PROCS  =         (int)((float)ONE_SECOND * 3.6);
const int NOISES      =         (int)((float)ONE_SECOND * 4.8);
const int WAYSLOW     =         2400;
const int REALHOUR    =         ONE_SECOND * 60 * 60;
const int REALDAY     =         ONE_SECOND * 60 * 60 * 24;

// Altering UPDATES will speed up ticks, but also causes "mud time"
// to totally recalculate (making it shorter will age people).
// use caution!
const int UPDATE      =         ONE_SECOND * 36;
const int MUDHOUR     =         UPDATE * 4;
const int MUDDAY      =         MUDHOUR * 24;

const int SECS_PER_UPDATE  =    UPDATE/ONE_SECOND;

const int SECS_PER_MUDHOUR  =   MUDHOUR/ONE_SECOND;
const int SECS_PER_MUD_DAY   =  (24 * SECS_PER_MUDHOUR);
const int SECS_PER_MUD_MONTH =  (28 * SECS_PER_MUD_DAY);
const int SECS_PER_MUD_YEAR  =  (12 * SECS_PER_MUD_MONTH);

// updateAffects() is called on combat counter (socket.cc)
// this will tell us how many combat-calls needed to simulate a "tick"
// The reason for this is that we sometimes want to say "it takes a mud-hour"
// and the mud-hour must be represented as a number of combat-pulse calls
//const int UPDATES_PER_TICK = (UPDATE/COMBAT);
const int UPDATES_PER_MUDHOUR = (MUDHOUR / COMBAT);

}

extern const char * const prompt_mesg[];
extern void signalSetup(void);

////


// this is the (abstract) base class for output messages
class Comm
{
 public:
  enum commTypeT { TEXT, CLIENT, XML };

  sstring getComm(commTypeT);

  virtual ~Comm(){}

  sstring text;

 protected:

 private:
  virtual sstring getText() = 0;
  virtual sstring getClientText() = 0;
  virtual sstring getXML() = 0;
};


class UncategorizedComm : public Comm {
 public:
  UncategorizedComm(const sstring &);

 private:
  virtual sstring getText();
  virtual sstring getClientText();
  virtual sstring getXML();
};

class WhoListComm : public Comm {
 public:
  WhoListComm(const sstring &, bool, int, int, bool, const sstring &, const sstring &);
  sstring who;
  bool online;
  int level;
  int idle;
  bool linkdead;
  sstring prof;
  sstring title;

 private:
  virtual sstring getText();
  virtual sstring getClientText();
  virtual sstring getXML();
};

class TellFromComm : public Comm {
 public:
  TellFromComm(const sstring &, const sstring &, const sstring &, bool, bool);

 private:
  sstring to;
  sstring from;
  bool drunk;
  bool mob;

  virtual sstring getText();
  virtual sstring getClientText();
  virtual sstring getXML();
};

class TellToComm : public Comm {
 public:
  TellToComm(const sstring &, const sstring &, const sstring &);

 private:
  sstring to;
  sstring from;

  virtual sstring getText();
  virtual sstring getClientText();
  virtual sstring getXML();
};

class CmdMsgComm : public Comm {
 public:
  CmdMsgComm(const sstring &, const sstring &);

 private:
  sstring cmd;

  virtual sstring getText();
  virtual sstring getClientText();
  virtual sstring getXML();
};


// for snoop output
class SnoopComm : public Comm {
 public:
  SnoopComm(const sstring &, const sstring &);

 private:
  sstring vict;

  virtual sstring getText();
  virtual sstring getClientText();
  virtual sstring getXML();
};

class LoginComm : public Comm {
 public:
  LoginComm(const sstring &, const sstring &);

 private:
  sstring prompt;

  virtual sstring getText();
  virtual sstring getClientText();
  virtual sstring getXML();
};

struct RoomExitData {
  bool exit;
  bool door;
  bool open;
};

class RoomExitComm : public Comm {
 public:
  RoomExitComm();
  RoomExitData exits[MAX_DIR];

 private:
  virtual sstring getText();
  virtual sstring getClientText();
  virtual sstring getXML();
};

class PromptComm : public Comm {
 public:
  PromptComm(time_t, int, int, float, int, int, int, int, const sstring &);
  time_t time;
  int hp;
  int mana;
  float piety;
  int lifeforce;
  int moves;
  int money;
  int room;

 private:
  virtual sstring getText();
  virtual sstring getClientText();
  virtual sstring getXML();
};



#endif
