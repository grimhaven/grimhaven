//   "comm.cc" - All functions and routines related to the central game
//               loop

#include <cstdio>
#include <csignal>
#include <cstdarg>

extern "C" {
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/param.h>
#include <arpa/inet.h>
#include <ares.h>
}

#include <boost/regex.hpp>

#include "misc/being.h"
#include "misc/person.h"
#include "misc/extern.h"
#include "misc/monster.h"
#include "misc/room.h"
#include "misc/account.h"
#include "sys/colorstring.h"
#include "sys/systemtask.h"
#include "sys/tsocket.h"
#include "spec/mobs.h"
#include "misc/weather.h"
#include "sys/ansi.h"
#include "sys/sstring.h"

const int PACKET_BUFFER_SIZE = 40960;
const int MAX_HOSTNAME =   256;
const int MAX_SOCKET_WRITE =  40960;

const int PROMPT_DONT_SEND = -1;

SystemTask *systask;

// local globals

char hostlist[MAX_BAN_HOSTS][40];        // list of sites to ban
char hostLogList[MAX_BAN_HOSTS][40];
int numberhosts;
int numberLogHosts;

extern void save_all();
extern ares_channel channel;

// Init sockets, run game, and cleanup sockets
int run_the_game()
{
  signalSetup();

  int ares_res = ares_library_init(ARES_LIB_INIT_ALL);
  if (ares_res) {
    vlogf(LOG_BUG, format("ares_library_init failed: %s") % ares_strerror(ares_res));
    return FALSE;
  }
  ares_res = ares_init(&channel);
  if (ares_res != ARES_SUCCESS) {
    vlogf(LOG_BUG, format("ares_init failed: %s") % ares_strerror(ares_res));
    return FALSE;
  }

  vlogf(LOG_MISC, format("Opening main socket on port %i") % Config.game_port());
  gSocket = new TMainSocket();
  gSocket->initSocket(Config.game_port());

  if (Config.xml_port()) {
    vlogf(LOG_MISC, format("Opening XML socket on port %i") % Config.xml_port());
    gSocket->initSocket(Config.xml_port());
  }

  bootDb();

  vlogf(LOG_MISC, "Entering game loop.");

  systask = new SystemTask();
  gSocket->gameLoop();
  gSocket->closeAllSockets();

  ares_destroy(channel);
  ares_library_cleanup();

  vlogf(LOG_MISC, "Normal termination of game.");
  delete gSocket;

  return FALSE;
}

void zoneData::nukeMobs()
{
  TThing *t;
  TBeing *mob, *mob2;
  wearSlotT i;

  if(!Config.NukeInactiveMobs())
    return;

  for (mob = character_list; mob; mob = mob2) {
    mob2 = mob->next;
    if (mob->isPc())
      continue;
    if (mob->specials.zone != zone_nr)
      continue;

    // preserve charms, mounts, etc
    if (mob->master && mob->master->isPc())
      continue;
    if ((mob->spec == SPEC_BOUNTY_HUNTER) || dynamic_cast<TMonster *>(mob)->hatefield)
      continue;
    if (mob->isRideable())
      continue;

    // nuke the mob and equipment
    for (i = MIN_WEAR; i < MAX_WEAR; i++) {
      if (mob->equipment[i]) {
        TThing *t_obj = mob->unequip(i);
        delete t_obj;
        t_obj = NULL;
      }
    }
    for(StuffIter it=mob->stuff.begin();it!=mob->stuff.end();){
      t=*(it++);
      delete t;
    }
    delete mob;
    mob = NULL;
  }
}

void TBeing::sendTo(Comm *c) const
{
  if (!desc)
    return;

  desc->output.putInQ(c);
}

void TBeing::sendTo(colorTypeT lev, const sstring &msg) const
{
  if (!desc)
    return;
  if (desc->connected == CON_WRITING)
    return;

  sstring messageBuffer = colorString(this, desc, msg, NULL, lev, FALSE);
  desc->output.putInQ(new UncategorizedComm(messageBuffer));
}

void TRoom::sendTo(colorTypeT lev, const sstring &text) const
{
  TThing *i;

  for(StuffIter it=stuff.begin();it!=stuff.end();++it) {
    i=*it;
    TBeing *tbt = dynamic_cast<TBeing *>(i);
    if (tbt && tbt->desc && !tbt->desc->connected) {
      if ((lev == COLOR_NEVER) || (lev == COLOR_NONE)) {
      } else {
        sstring messageBuffer = colorString(tbt, i->desc, text, NULL, lev, TRUE);
        tbt->desc->output.putInQ(new UncategorizedComm(messageBuffer));
      }
    }
  }
}

void TThing::sendTo(colorTypeT, const sstring &msg) const
{
}


void TBeing::sendTo(const sstring &msg) const
{
  if (msg.empty() || !desc)
    return;

  if (desc->connected == CON_WRITING)
    return;

  desc->output.putInQ(new UncategorizedComm(msg));
}

void save_all()
{
  descriptor_list->saveAll();
}

void sendToOutdoor(colorTypeT lev, const sstring &text, const sstring &text_tropic)
{
  Descriptor *i;
  TBeing *ch;

  if (!text.empty()) {
    for (i = descriptor_list; i; i = i->next) {
      if (!i->connected && (ch = i->character)) {
        if (ch->outside() && ch->awake() && ch->roomp  &&
              !(ch->isPlayerAction(PLR_MAILING | PLR_BUGGING))) {
          if ((lev == COLOR_NEVER) || (lev == COLOR_NONE)) {
            if (ch->roomp->isTropicalSector()) {
              i->output.putInQ(new UncategorizedComm(text_tropic));
            } else {
              i->output.putInQ(new UncategorizedComm(text));
            }
          } else {
            sstring buf;
            if (ch->roomp->isTropicalSector()) {
              buf = colorString(ch, i, text_tropic, NULL, lev, FALSE);
            } else {
              buf = colorString(ch, i, text, NULL, lev, FALSE);
            }
            i->output.putInQ(new UncategorizedComm(buf));
          }
        }
      }
    }
  }
}


void sendToExcept(char *text, TBeing *ch)
{
  Descriptor *i;

  if (text)
    for (i = descriptor_list; i; i = i->next)
      if (ch->desc != i && !i->connected)
        i->output.putInQ(new UncategorizedComm(text));
}

void sendToRoom(colorTypeT color, const char *text, int room)
{
  TThing *i=NULL;

  if (!real_roomp(room)) {
    vlogf(LOG_MISC, format("BOGUS room %d in sendToRoom") %  room);
    return;
  }
  if (text) {
    for(StuffIter it=real_roomp(room)->stuff.begin();it!=real_roomp(room)->stuff.end() && (i=*it);++it) {
      TBeing *tbt = dynamic_cast<TBeing *>(i);
      if (tbt && tbt->desc && !tbt->desc->connected && tbt->awake()) {
        sstring buf = colorString(tbt, tbt->desc, text, NULL, color, FALSE);
        tbt->desc->output.putInQ(new UncategorizedComm(buf));
      }
    }
  }
}

void sendToRoom(const char *text, int room)
{
  TThing *i=NULL;

  if (!real_roomp(room)) {
    vlogf(LOG_MISC, format("BOGUS room %d in sendToRoom") %  room);
    return;
  }
  if (text) {
    for(StuffIter it=real_roomp(room)->stuff.begin();it!=real_roomp(room)->stuff.end() && (i=*it);++it) {
      TBeing *tbt = dynamic_cast<TBeing *>(i);
      if (!tbt)
        continue;
      if (tbt->desc && !tbt->desc->connected && tbt->awake())
        tbt->desc->output.putInQ(new UncategorizedComm(text));
    }
  }
}

void sendrpf(colorTypeT color, TRoom *rp, const char *msg, ...)
{
  char messageBuffer[MAX_STRING_LENGTH];
  va_list ap;

  if (rp && msg) {
    va_start(ap, msg);
    vsnprintf(messageBuffer, cElements(messageBuffer), msg, ap);
    va_end(ap);

    sendrpf(0, color, rp, messageBuffer);
  }
}

void sendrpf(int tslevel, colorTypeT color, TRoom *rp, const char *msg,...)
{
  char messageBuffer[MAX_STRING_LENGTH];
  va_list ap;
  TThing *i=NULL;

  if (rp && msg) {
    va_start(ap, msg);
    vsnprintf(messageBuffer, cElements(messageBuffer), msg, ap);
    va_end(ap);
    for(StuffIter it=rp->stuff.begin();it!=rp->stuff.end() && (i=*it);++it) {
      TBeing *tbt = dynamic_cast<TBeing *>(i);
      if (tbt && tbt->desc && !tbt->desc->connected && tbt->awake() &&
          tbt->GetMaxLevel() > tslevel)
        tbt->desc->output.putInQ(new UncategorizedComm(colorString(tbt, tbt->desc, messageBuffer, NULL, color, TRUE)));
    }
  }
}

void sendrpf(TRoom *rp, const char *msg, ...)
{
  char messageBuffer[MAX_STRING_LENGTH];
  va_list ap;

  if (rp && msg) {
    va_start(ap, msg);
    vsnprintf(messageBuffer, cElements(messageBuffer), msg, ap);
    va_end(ap);

    sendrpf(0, rp, messageBuffer);
  }
}

void sendrpf(int tslevel, TRoom *rp, const char *msg,...)
{
  char messageBuffer[MAX_STRING_LENGTH];
  va_list ap;
  TThing *i=NULL;

  if (rp && msg) {
    va_start(ap, msg);
    vsnprintf(messageBuffer, cElements(messageBuffer), msg, ap);
    va_end(ap);
    for(StuffIter it=rp->stuff.begin();it!=rp->stuff.end() && (i=*it);++it) {
      TBeing *tbt = dynamic_cast<TBeing *>(i);
      if (tbt && tbt->desc && !tbt->desc->connected && tbt->awake() &&
          tbt->GetMaxLevel() > tslevel)
        tbt->desc->output.putInQ(new UncategorizedComm(colorString(tbt, tbt->desc, messageBuffer, NULL, COLOR_NONE, TRUE)));

    }
  }
}


void TRoom::sendTo(const sstring &text) const
{
  TThing *i;

  for(StuffIter it=stuff.begin();it!=stuff.end();++it) {
    i=*it;
    if (i->desc && !i->desc->connected)
      i->desc->output.putInQ(new UncategorizedComm(text));
  }
}

void TThing::sendTo(const sstring &) const
{
}

void colorAct(colorTypeT colorLevel, const sstring &str, bool hide, const TThing *t1, const TThing *obj, const TThing *t3, actToParmT type, const char *color, int tslevel)
{
  bool colorize = 0;
  const TThing *to;
  int which = 0;
  char buf[256];


  if(str.empty())
    return;

  if (!t1) {
    vlogf(LOG_MISC, "There is no char in coloract TOCHAR.");
    vlogf(LOG_MISC, format("%s") %  str);
    return;
  }

  if (!t1->roomp)
    return;

  if (!t3) {
    if (type == TO_VICT) {
      vlogf(LOG_MISC, format("There is no victim in coloract TOVICT %s is char.") %  t1->getName());
      vlogf(LOG_MISC, format("%s") %  str);
      return;
    } else if (type == TO_NOTVICT) {
      type = TO_ROOM;
      vlogf(LOG_MISC, format("There is no victim in coloract TONOTVICT %s is char.") %  t1->getName());
      vlogf(LOG_MISC, format("%s") %  str);
    }
  }

  if (type == TO_VICT) {
    to = t3;
    which = TRUE;
  } else if (type == TO_CHAR) {
    to = t1;
    which = TRUE;
  } else {
    which = FALSE;
    to = t1->roomp->stuff.front();
  }

  if (!to->desc && ((type == TO_VICT) || (type == TO_CHAR)))
    return;

  if (which) {
    switch (colorLevel) {
      case COLOR_ALWAYS:
      case COLOR_BASIC:   //allows for basic ansi
        colorize = TRUE;
        break;
      case COLOR_NONE:
      case COLOR_NEVER:
        colorize = FALSE;
        break;
      case COLOR_COMM:
        if (!(IS_SET(to->desc->plr_color, PLR_COLOR_COMM))) {
           colorize = FALSE;
        } else {
          colorize = TRUE;
        }
        break;
      case COLOR_OBJECTS:
        if (!(IS_SET(to->desc->plr_color, PLR_COLOR_OBJECTS)))
           colorize = FALSE;
        else
          colorize = TRUE;

        break;
      case COLOR_MOBS:
        if (!(IS_SET(to->desc->plr_color, PLR_COLOR_MOBS))) {
           colorize = FALSE;
        } else {
          colorize = TRUE;
        }
        break;
      case COLOR_ROOMS:
        if (!(IS_SET(to->desc->plr_color, PLR_COLOR_ROOMS))) {
           colorize = FALSE;
        } else {
          colorize = TRUE;
        }
        break;
      case COLOR_ROOM_NAME:
        if (!(IS_SET(to->desc->plr_color, PLR_COLOR_ROOM_NAME))) {
           colorize = FALSE;
        } else {
          colorize = TRUE;
        }
        break;
      case COLOR_SHOUTS:
        if (!(IS_SET(to->desc->plr_color, PLR_COLOR_SHOUTS))) {
           colorize = FALSE;
        } else {
          colorize = TRUE;
        }
        break;
      case COLOR_SPELLS:
        if (!(IS_SET(to->desc->plr_color, PLR_COLOR_SPELLS))) {
           colorize = FALSE;
        } else {
          colorize = TRUE;
        }
        break;
      case COLOR_LOGS:
        if (!(IS_SET(to->desc->plr_color, PLR_COLOR_LOGS))) {
           colorize = FALSE;
        } else {
          colorize = TRUE;
        }
        break;
      default:
        vlogf(LOG_MISC, "colorAct with a default COLOR setting");
        colorize = TRUE;
        break;
    }
    if (colorize) {
      snprintf(buf, cElements(buf), "%s", str.c_str());
    } else {
      snprintf(buf, cElements(buf), "%s", colorString(dynamic_cast<const TBeing *>(to), to->desc, str.c_str(), NULL, COLOR_NONE, TRUE).c_str());

    }
    act(buf, hide, t1, obj, t3, type, color);
  } else {
// TO_ROOM
// Doesnt work well if there are substitutes but if none its ok
    for(StuffIter it=t1->roomp->stuff.begin();it!=t1->roomp->stuff.end();++it){
      const TBeing *tbto = dynamic_cast<const TBeing *>(*it);
      if (!tbto || !tbto->desc || tbto->GetMaxLevel() <= tslevel)
        continue;

      if (tbto == t1)
        continue;

      if (tbto == t3 && type == TO_NOTVICT)
        continue;

      colorAct(colorLevel, str, hide, t1, obj, tbto, TO_VICT, color, tslevel);
    }
  }
}


void act(const sstring &str, bool hide, const TThing *t1, const TThing *obj, const TThing *t3, actToParmT type, const char *color, int tslevel)
{
  register const char *strp;
  register char *point;
  register const char *i = NULL;
  char ibuf[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  char namebuf[MAX_NAME_LENGTH];
  char lastColor[3];
  const char *codes = NULL;
  const char *codes2 = NULL;
  int x = 0;
  personTypeT per;
  const TObj *tobj = NULL;
  sstring catstr;

  if(str.empty())
    return;

  if (!t1) {
    vlogf(LOG_MISC, "There is no char in act() TOCHAR.");
    vlogf(LOG_MISC, format("%s") %  str);
    return;
  }
  if (!t1->roomp){
    //    vlogf(LOG_MISC, "There is no room in act() TOCHAR");
    //    vlogf(LOG_MISC, format("%s") %  str);
    return;
  }

  if (!t3) {
    if (type == TO_VICT) {
      vlogf(LOG_MISC, format("There is no victim in act() TOVICT %s is char.") %  t1->getName());
      vlogf(LOG_MISC, format("%s") %  str);
      return;
    } else if (type == TO_NOTVICT) {
      type = TO_ROOM;
      vlogf(LOG_MISC, format("There is no victim in act() TONOTVICT %s is char.") %  t1->getName());
      vlogf(LOG_MISC, format("%s") %  str);
    }
  }

  StuffListConst list;

  if (type == TO_VICT) {
    list.push_front(t3);
  } else if (type == TO_CHAR) {
    list.push_front(t1);
  } else {
    if (!t1->roomp){
      //      vlogf(LOG_MISC, "There is no room in act() TOCHAR 2");
      //      vlogf(LOG_MISC, format("%s") %  str);
      return;
    }

    for(StuffIter it=t1->roomp->stuff.begin();it!=t1->roomp->stuff.end();++it){
      list.push_front(*it);
    }
  }

  memset(&buf, '\0', sizeof(buf));
  memset(lastColor, '\0', sizeof(lastColor));
  for(StuffIterConst it=list.begin();it!=list.end();++it){
    const TBeing *to = dynamic_cast<const TBeing *>(*it);

    if (to && to->desc && to->GetMaxLevel() > tslevel &&
          ((to != t1) || (type == TO_CHAR)) &&
          ((to != t3 || (t1 == t3 && type == TO_CHAR)) ||
               (type == TO_VICT) || (type == TO_ROOM)) &&
        (to->canSee(t1) || !hide) &&
        to->awake() && (to->desc->connected < MAX_CON_STATUS) &&
        !(to->isPlayerAction(PLR_MAILING | PLR_BUGGING))) {
      x = 0; // used to determine whether or not to capitalize the substitution at start of line
      for (strp = str.c_str(), point = buf;;) {
        x = x + 1;
        codes = strp;
        if ((*codes == '<') && (*(++codes) != '<')) {
          codes2 = codes;
          if (*(++codes) == '>') {
              lastColor[0] = '<';
              lastColor[1] = *codes2;
              lastColor[2] = '>';
          }
        }
        if (*strp == '$') {
          const TBeing * tbtt;
          switch (*(++strp)) {
            case 'n':
              tbtt = dynamic_cast<const TBeing *>(t1);
              i = tbtt ? to->pers(t1) : to->objs(t1);
              if (x == 1 || (x == 4 && *lastColor)) {
                strncpy(namebuf, i, cElements(namebuf));
                strncpy(namebuf, sstring(namebuf).cap().c_str(), cElements(namebuf));
                i = namebuf;
              }
              strncpy(ibuf, colorString(to, to->desc, i, NULL, tbtt ? COLOR_MOBS : COLOR_OBJECTS, FALSE).c_str(), cElements(ibuf));
              i=ibuf;
              break;
            case 'P':
            case 'N':
              if (!t3) {
                vlogf(LOG_BUG, format("Bad act P or N. '%s'") %  str);
                return;
              }
              tbtt = dynamic_cast<const TBeing *>(t3);
              i = tbtt ? to->pers(t3) : to->objs(t3);
              if (x == 1 || (x == 4 && *lastColor)) {
                strncpy(namebuf, i, cElements(namebuf));
                strncpy(namebuf, sstring(namebuf).cap().c_str(), cElements(namebuf));
                i = namebuf;
              }
              if ((type == TO_CHAR) && (t1 == t3)) {
                if (!strncmp(strp+1,"'s ",3)) {
                  i = "your";
                  strp += 2;
                } else if (!strncmp(strp+1," is",3)) {
                  i = "you are";
                  strp += 3;
                } else {
                  // first word = "You", otherwise "Yourself"
                  if (strlen(buf))
                    i = "yourself";
                  else
                    i = "you";
                }
              } else if ((type == TO_NOTVICT) && (t1 == t3)) {
                if (!strncmp(strp+1,"'s ",3)) {
                  i = t1->hshr();
                  strp += 2;
                } else if (strp != (str.c_str() + 1)) {
                  // "himself" if it isn't the first word in the sstring
                  char tmp_buffer[20];
                  snprintf(tmp_buffer, cElements(tmp_buffer), "%sself", t1->hmhr());
                  i = tmp_buffer;
                }
              }
              strncpy(ibuf,colorString(to, to->desc, i, NULL, tbtt ? COLOR_MOBS : COLOR_OBJECTS, FALSE).c_str(), cElements(ibuf));
              i=ibuf;

              break;
            case 'g':
              strncpy(ibuf, t1->roomp->describeGround().c_str(), cElements(ibuf));
              i=ibuf;
              break;
            case 'G':
              if (!t3) {
                vlogf(LOG_BUG, format("Bad act G. '%s'") %  str);
                return;
              }
              strncpy(ibuf, t3->roomp->describeGround().c_str(), cElements(ibuf));
              i=ibuf;
              break;
            case 'd':
              per = ((to == t1) ? FIRST_PERSON : (!strlen(buf) ? THIRD_PERSON : SECOND_PERSON));
              strncpy(ibuf, t1->yourDeity(your_deity_val, per, (per == THIRD_PERSON) ? to : NULL).c_str(), cElements(ibuf));
              i=ibuf;
              break;
            case 'D':
              if (!t3) {
                vlogf(LOG_BUG, format("Bad act D. '%s'") %  str);
                return;
              }
              strncpy(ibuf, t3->yourDeity(your_deity_val, ((to == t3) ? FIRST_PERSON : (strlen(buf) == 0 ? THIRD_PERSON : SECOND_PERSON))).c_str(), cElements(ibuf));
              i=ibuf;
              break;
            case 'q':
              // is/are based on plurality of $o, $p
              if (!obj) {
                vlogf(LOG_BUG, format("Bad act q. '%s'") %  str);
                return;
              }
              tobj = dynamic_cast<const TObj *>(obj);
              if (tobj)
                i = tobj->isPluralItem() ? "are" : "is";
              else
                i = "is";
              break;
            case 'Q':
              // a verb modifier so can do "$o look$Q happy" for plurality
              if (!obj) {
                vlogf(LOG_BUG, format("Bad act Q. '%s'") %  str);
                return;
              }
              tobj = dynamic_cast<const TObj *>(obj);
              if (tobj)
                i = tobj->isPluralItem() ? "" : "s";
              else
                i = "s";
              break;
            case 'r':
              // is/are based on plurality of $n
              if (!t1) {
                vlogf(LOG_BUG, format("Bad act r. '%s'") %  str);
                return;
              }
              tobj = dynamic_cast<const TObj *>(t1);
              if (tobj)
                i = tobj->isPluralItem() ? "are" : "is";
              else
                i = "is";
              break;
            case 'R':
              // a verb modifier so can do "$n look$Q happy" for plurality
              if (!t1) {
                vlogf(LOG_BUG, format("Bad act R. '%s'") %  str);
                return;
              }
              tobj = dynamic_cast<const TObj *>(t1);
              if (tobj)
                i = tobj->isPluralItem() ? "" : "s";
              else
                i = "s";
              break;
            case 'm':
              if (to->canSee(t1))
                i = t1->hmhr();
              else
                i = "someone";
              break;
            case 'M':
              if (!t3) {
                vlogf(LOG_BUG, format("Bad act M. '%s'") %  str);
                return;
              }
              if ((type == TO_CHAR) && (t1 == t3))
                i = "yourself";
              else if (to->canSee(t3))
                i = t3->hmhr();
              else
                i = "someone";
              break;
            case 's':
              if (to->canSee(t1))
                i = t1->hshr();
              else
                i = "their";
              break;
            case 'S':
              if (!t3) {
                vlogf(LOG_BUG, format("Bad act S. '%s'") %  str);
                return;
              }
              if (to->canSee(t3))
                i = t3->hshr();
              else
                i = "their";
              break;
            case 'e':
              if (to->canSee(t1))
                i = t1->hssh();
              else
                i = "it";
              break;
            case 'E':
              if (!t3) {
                vlogf(LOG_BUG, format("Bad act E. '%s'") %  str);
                return;
              }
              if (to->canSee(t3))
                i = t3->hssh();
              else
                i = "it";
              break;
            case 'o':
              if (!obj) {
                vlogf(LOG_BUG, format("Bad act o. '%s'") %  str);
                return;
              }
              strncpy(ibuf, dynamic_cast<const TBeing *>(obj) ? to->persfname(obj).c_str() : to->objn(obj).c_str(), cElements(ibuf));
              i=ibuf;
              break;
            case 'O':
              if (!t3) {
                vlogf(LOG_BUG, format("Bad act O. '%s'") %  str);
                return;
              }
              strncpy(ibuf, dynamic_cast<const TBeing *>(t3) ? to->persfname(t3).c_str() : to->objn(t3).c_str(), cElements(ibuf));
              i=ibuf;
              break;
            case 'p':
              if (!obj) {
                vlogf(LOG_BUG, format("Bad act p. '%s'") %  str);
                return;
              }
              tbtt = dynamic_cast<const TBeing *>(obj);
              i = tbtt ? to->pers(obj) : to->objs(obj);
              if (x == 1 || (x == 4 && *lastColor)) {
                strncpy(namebuf, i, cElements(namebuf));
                strncpy(namebuf, sstring(namebuf).cap().c_str(), cElements(namebuf));
                i = namebuf;
              }
              strncpy(ibuf, colorString(to, to->desc, i, NULL, tbtt ? COLOR_MOBS : COLOR_OBJECTS, FALSE).c_str(), cElements(ibuf));
              i=ibuf;
              break;
            case 'a':
              if (!obj) {
                vlogf(LOG_BUG, format("Bad act a. '%s'") %  str);
                return;
              }
              i = obj->sana();
              break;
            case 'A':
              if (!t3) {
                vlogf(LOG_BUG, format("Bad act A. '%s'") %  str);
                return;
              }
              i = t3->sana();
              break;
            case 'T':
              if (!t3) {
                vlogf(LOG_BUG, format("Bad act T. '%s'") %  str);
                return;
              }
              i = (const char *) t3;
              break;
            case 'F':
              if (!t3) {
                vlogf(LOG_BUG, format("Bad act F. '%s'") %  str);
                return;
              }
              i = fname((const char *) t3).c_str();
              break;
            case '$':
              i = "$";
              break;
            default:
              i = "$";
              break;
          }
          // color in the replacement sstring may reset existing color
          // to get around this, lets tack on any existing color
          if (color) {
            catstr = i;
            catstr += to->ansi_color(color);
            strncpy(ibuf, catstr.c_str(), cElements(ibuf));
            i=ibuf;
          }
          while ((*point = *(i++)) != 0)
            ++point;
          ++strp;

        } else if (!(*(point++) = *(strp++)))
          break;
      }

      // we used to put the \n\r pad on here, but this causes the optional
      // color codes to be left dangling on the next line, causing some
      // problems for the client
      *(--point) = '\0';

      if (!((to->GetMaxLevel() > MAX_MORT) &&
          (IS_SET(to->desc->plr_color, PLR_COLOR_CODES)))) {
        snprintf(buf, cElements(buf), "%s", colorString(to, to->desc, buf, NULL, COLOR_BASIC, FALSE).c_str());
      }

      sstring s=buf;

      if (!color) {
        to->desc->output.putInQ(new UncategorizedComm(format("%s\n\r") %s.cap()));
      } else {
        sstring str = to->ansi_color(color);
        if (str.empty())
          to->desc->output.putInQ(new UncategorizedComm(format("%s\n\r") %s.cap()));
        else {
          to->desc->output.putInQ(new UncategorizedComm(format("%s%s%s\n\r") %
                                                        str % s.cap() %
                                                        to->norm()));
        }
      }
    }
    if ((type == TO_VICT) || (type == TO_CHAR))
      return;
  }
}

void Descriptor::updateScreenVt100(unsigned int update)
{
  char buf[4096];
  TBeing *f, *ch;

  if (!(ch = character))
    return;

  if (!ch->vt100()) {
    vlogf(LOG_MISC, format("%s in updateScreenVt100 and not vt (%d)") % ch->getName() %update);
    return;
  }

  if (!ch->desc || !IS_SET(ch->desc->prompt_d.type, PROMPT_VTANSI_BAR))
    return;

  writeToQ(VT_CURSAVE);
  snprintf(buf, cElements(buf), VT_CURSPOS, ch->getScreen() - 3, 1);

  if (IS_SET(prompt_d.type, PROMPT_CLASSIC_ANSIBAR)) {

    // Line 1:

    if (update & CHANGED_HP) {
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 2, 7);
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%d", ch->getHit());
    }

    if (ch->hasClass(CLASS_DEIKHAN) || ch->hasClass(CLASS_CLERIC)) {
      if (update & CHANGED_PIETY) {
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 2, 34);
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%.1f", ch->getPiety());
      }
    } else if (ch->hasClass(CLASS_SHAMAN)) {
      if (update & CHANGED_LIFEFORCE) {
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 2, 34);
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%d", ch->getLifeforce());
      }
    } else {
      if (update & CHANGED_MANA) {
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 2, 34);
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%d", ch->getMana());
      }
    }

    if (update & CHANGED_MOVE) {
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 2, 62);
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%d", ch->getMove());
    }

    // Line 2:

    if (ch->isImmortal()) {
      if (update & CHANGED_ROOM) {
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 1, 7);
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%d", ch->roomp->number);
      }
    } else {
#if FACTIONS_IN_USE
      if (update & CHANGED_PERC) {
        sprintf(buf + strlen(buf), VT_CURSPOS, ch->getScreen() - 1, 7);
        sprintf(buf + strlen(buf), "%3.4f", ch->getPerc());
      }
#endif
    }

    if (update & CHANGED_GOLD) {
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 1, 34);
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%d", ch->getMoney());
    }

    if (update & CHANGED_EXP) {
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 1, 58);
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%s", ch->displayExp().c_str());
    }

    // Line 3:

    if ((f = ch->fight()) != NULL) {
      if (f->sameRoom(*ch)) {
        int maxh = max(1, (int) f->hitLimit());
        int ratio = min(10, max(0, ((f->getHit() * 9) / maxh)));
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen(), 3);
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "<%s=%s>", fname(f->name).c_str(), prompt_mesg[ratio]);
        last.fighting = TRUE;

        ratio = fname(f->name).length() + strlen(prompt_mesg[ratio]);

        while (ratio < 25) {
          strncat(buf, " ", cElements(buf));
          ratio++;
        }
      }
    } else {
      if (last.fighting) {
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen(), 3);
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "                         ");
        last.fighting = FALSE;
      }
    }

    if (IS_SET(update, CHANGED_MUD)) {
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen(), 35);
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), " %s ",
               GameTime::hmtAsString(GameTime::hourminTime()).c_str());
    }

    if (IS_SET(update, CHANGED_TIME)) {
      time_t t1;
      struct tm *tptr;
      if ((t1 = time(0)) != -1) {
        tptr = localtime(&t1);

        // adjust time for users local site
        tptr->tm_hour += account->time_adjust;
        if (tptr->tm_hour < 0) {
          tptr->tm_hour += 24;
        } else if (tptr->tm_hour > 23)
          tptr->tm_hour -= 24;

        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen(), 62);
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%2d:%02d %2s",
          (!(tptr->tm_hour%12) ? 12 : tptr->tm_hour%12),
          tptr->tm_min,
          (tptr->tm_hour >= 12) ? "PM" : "AM");
      }
    }

  } else {

    // Line 1:

    if (update & CHANGED_HP) {
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 2, 7);
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%-5d", ch->getHit());
    }

    if (ch->hasClass(CLASS_DEIKHAN) || ch->hasClass(CLASS_CLERIC)) {
      if (update & CHANGED_PIETY) {
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 2, 34);
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%.1f", ch->getPiety());
      }
    } else if (ch->hasClass(CLASS_SHAMAN)) {
      if (update & CHANGED_LIFEFORCE) {
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 2, 34);
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%-4d", ch->getLifeforce());
      }
    } else {
      if (update & CHANGED_MANA) {
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 2, 34);
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%-4d", ch->getMana());
      }
    }

    if (update & CHANGED_MOVE) {
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 2, 62);
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%-4d", ch->getMove());
    }

    // Line 2:

    if ((f = ch->fight()) != NULL) {
      if (f->sameRoom(*ch)) {
        char StTemp[120];
        int maxh = max(1, (int) f->hitLimit());
        int ratio = min(10, max(0, ((f->getHit() * 9) / maxh)));

        memset(&StTemp, 0, sizeof(StTemp));
        snprintf(StTemp, cElements(StTemp), "<%s=%s>", fname(f->name).c_str(), prompt_mesg[ratio]);

        if (strlen(StTemp) > 22) {
          StTemp[19] = StTemp[20] = StTemp[21] = '.';
          StTemp[22] = '>';
          StTemp[23] = '\0';
        }

        for (int iRunner = strlen(StTemp); iRunner < 23; iRunner++)
          StTemp[iRunner] = ' ';

        StTemp[23] = '\0';

        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 1, 1);
        strncat(buf + strlen(buf), StTemp, cElements(buf) - strlen(buf));

        last.fighting = TRUE;
      }
    } else {
      if (last.fighting) {
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 1, 1);
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "                      ");
        last.fighting = FALSE;
      }
    }

    if (update & CHANGED_EXP) {
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 1, 34);
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%s", ch->displayExp().c_str());
    }

    if (ch->isImmortal()) {
      if (update & CHANGED_ROOM) {
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 1, 62);
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%-6d", ch->roomp->number);
      }
    } else {
#if FACTIONS_IN_USE
      if (update & CHANGED_PERC) {
        sprintf(buf + strlen(buf), VT_CURSPOS, ch->getScreen() - 1, 62);
        sprintf(buf + strlen(buf), "%3.4f", ch->getPerc());
      }
#else
      if (ch->fight() && ch->awake() && ch->fight()->sameRoom(*ch)) {
        f = ch->fight()->fight();

        if (f && (f != ch) && ch->sameRoom(*f)) {
          char StTemp[120];
          int ratio = min(10, max(0, ((f->getHit() * 9) / f->hitLimit())));

          memset(&StTemp, 0, sizeof(StTemp));
          snprintf(StTemp, cElements(StTemp), "<%s=%s>", fname(f->name).c_str(), prompt_mesg[ratio]);

          if (strlen(StTemp) > 22) {
            StTemp[19] = StTemp[20] = StTemp[21] = '.';
            StTemp[22] = '>';
            StTemp[23] = '\0';
          }

          for (int iRunner = strlen(StTemp); iRunner < 23; iRunner++)
            StTemp[iRunner] = ' ';

          StTemp[23] = '\0';

          snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 1, 55);
          strncat(buf + strlen(buf), StTemp, cElements(buf) - strlen(buf));

          last.fighting = TRUE;
        } else {
          snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 1, 55);
          snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "                      ");
        }
      } else {
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen() - 1, 55);
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "                      ");
      }
#endif
    }

    // Line 3:

    if (IS_SET(update, CHANGED_MUD)) {
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen(), 1);
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "   %8s   ", GameTime::hmtAsString(GameTime::hourminTime()).c_str());
    }

    if (IS_SET(update, CHANGED_TIME)) {
      time_t t1;
      struct tm *tptr;
      if ((t1 = time(0)) != -1) {
        tptr = localtime(&t1);

        // adjust time for users local site
        tptr->tm_hour += account->time_adjust;
        if (tptr->tm_hour < 0) {
          tptr->tm_hour += 24;
        } else if (tptr->tm_hour > 23)
          tptr->tm_hour -= 24;

        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen(), 15);
        snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "   %2d:%02d %2s   ",
                (!(tptr->tm_hour%12) ? 12 : tptr->tm_hour%12),
                tptr->tm_min,
                (tptr->tm_hour >= 12) ? "PM" : "AM");
      }
    }

    if (update & CHANGED_EXP) {
      classIndT iClass;

      for (iClass = MAGE_LEVEL_IND; iClass < MAX_CLASSES; iClass++) {
        if (ch->getLevel(iClass)) {
          double iNeed = getExpClassLevel(iClass, ch->getLevel(iClass) + 1) - ch->getExp();

          snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen(), 34);

          if (ch->getLevel(iClass) >= MAX_MORT)
            strncat(buf + strlen(buf), "0", cElements(buf) - strlen(buf));
          else {
            char StTemp[15];

            memset(&StTemp, 0, sizeof(StTemp));

            if (ch->getExp() < 100)
              snprintf(StTemp, cElements(StTemp), "%.3f", iNeed);
            else
              snprintf(StTemp, cElements(StTemp), "%.0f", iNeed);

            for (int iRunner = strlen(StTemp); iRunner < 11; iRunner++)
              StTemp[iRunner] = ' ';

            StTemp[11] = '\0';

            strncat(buf + strlen(buf), StTemp, cElements(buf) - strlen(buf));
          }

          break;
        }
      }
    }

    if (update & CHANGED_GOLD) {
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), VT_CURSPOS, ch->getScreen(), 62);
      snprintf(buf + strlen(buf), cElements(buf) - strlen(buf), "%-8d", ch->getMoney());
    }

  }

  writeToQ(buf);
  writeToQ(VT_NORMALT);
  writeToQ(VT_CURREST);

  // Make sure it doesn't send a new '>' when automatically updated - Russ
  if ((update == CHANGED_TIME) || (update == CHANGED_MUD))
    prompt_mode = PROMPT_DONT_SEND;
}

void Descriptor::updateScreenAnsi(unsigned int update)
{
  sstring buf;
  int missing_hit, missing_mana, missing_moves;
  int current_hit, current_mana, current_moves;
  int i;
  TBeing *f, *ch;

  if (!(ch = character))
    return;

  if (!ch->ansi()) {
    vlogf(LOG_MISC, format("%s in updateScreenAnsi and not ansi (%d)") % ch->getName() % update);
    return;
  }

  if (!ch->desc || !IS_SET(ch->desc->prompt_d.type, PROMPT_VTANSI_BAR))
    return;

  current_hit = (int) (10 * ((double) ch->getHit() / (double) ch->hitLimit()));

  if (ch->hasClass(CLASS_DEIKHAN) || ch->hasClass(CLASS_CLERIC))
    current_mana = (int) (10 * ch->getPiety() / 100.0);
  else if (ch->hasClass(CLASS_SHAMAN))
    current_mana = (int) (10 * ((double) ch->getLifeforce()));
  else
    current_mana = (int) (10 * ((double) ch->getMana() / (double) ch->manaLimit()));

  current_moves = (int) (10 * ((double) ch->getMove() / (double) ch->moveLimit()));
  current_hit = max(0, min(10, current_hit));

  if (ch->hasClass(CLASS_DEIKHAN) || ch->hasClass(CLASS_CLERIC))
    current_mana = (int) max(0., min(10., (double) current_mana));
  else if (ch->hasClass(CLASS_SHAMAN))
    current_mana = max(0, min(10, current_mana));
  else
    current_mana = max(0, min(10, current_mana));

  current_moves = max(0, min(10, current_moves));
  missing_hit = 10 - current_hit;
  missing_mana = 10 - current_mana;
  missing_moves = 10 - current_moves;

  writeToQ(VT_CURSAVE);
  buf = format(VT_CURSPOS) % (ch->getScreen() - 2) % 1;

  if (IS_SET(prompt_d.type, PROMPT_CLASSIC_ANSIBAR)) {

    // Line 1:

    if (update & CHANGED_HP) {
      buf += format(VT_CURSPOS) % (ch->getScreen() - 2) % 7;
      buf += format("%s%-5d ") % (current_hit > 2 ? VT_BOLDTEX : ANSI_RED) % ch->getHit();
      buf += ANSI_BLUE;

      for (i = 1; i <= current_hit; i++)
        buf += ANSI_BAR3;

      buf += ANSI_CYAN;

      for (i = 1; i <= missing_hit; i++)
        buf += ANSI_BAR3;
    }

    if (IS_SET(update, CHANGED_MANA) || IS_SET(update, CHANGED_PIETY) || IS_SET(update, CHANGED_LIFEFORCE)) {
      buf += format(VT_CURSPOS) % (ch->getScreen() - 2) % 34;

      if (ch->hasClass(CLASS_DEIKHAN) || ch->hasClass(CLASS_CLERIC))
        buf += format("%s%-5.1f ") % (current_mana ? VT_BOLDTEX : ANSI_RED) % ch->getPiety();
      else if (ch->hasClass(CLASS_SHAMAN))
        buf += format("%s%-5d ") % (current_mana ? VT_BOLDTEX : ANSI_RED) % ch->getLifeforce();
      else
        buf += format("%s%-5d ") % (current_mana ? VT_BOLDTEX : ANSI_RED) % ch->getMana();

      buf += ANSI_BLUE;

      for (i = 1; i <= current_mana; i++)
       buf += ANSI_BAR3;

      buf += ANSI_CYAN;

      for (i = 1; i <= missing_mana; i++)
        buf += ANSI_BAR3;
    }

    if (IS_SET(update, CHANGED_MOVE)) {
      buf += format(VT_CURSPOS) % (ch->getScreen() - 2) % 62;
      buf += format("%s%-5d ") % (current_moves ? VT_BOLDTEX : ANSI_RED) % ch->getMove();
      buf += ANSI_BLUE;

      for (i = 1; i <= current_moves; i++)
        buf += ANSI_BAR3;

      buf += ANSI_CYAN;

      for (i = 1; i <= missing_moves; i++)
        buf += ANSI_BAR3;
    }

    // Line 2:

    if (ch->isImmortal()) {
      if (update & CHANGED_ROOM) {
        buf += format(VT_CURSPOS) % (ch->getScreen() - 1) % 7;
        buf += format("%s%-6d") % ANSI_GREEN % ch->roomp->number;
      }
    } else {
#if FACTIONS_IN_USE
      if (update & CHANGED_PERC) {
        sprintf(buf + strlen(buf), VT_CURSPOS, ch->getScreen() - 1, 7);
        sprintf(buf + strlen(buf), "%3.4f", ch->getPerc());
      }
#endif
    }

    if (IS_SET(update, CHANGED_GOLD)) {
      buf += format(VT_CURSPOS) % (ch->getScreen() - 1) % 34;
      buf += format("%s%-8d") % ANSI_GREEN % ch->getMoney();
    }

    if (IS_SET(update, CHANGED_EXP)) {
      buf += format(VT_CURSPOS) % (ch->getScreen() - 1) % 58;
      buf += format("%s%s") % ANSI_GREEN % ch->displayExp();
    }

    // Line 3:

    if ((f = ch->fight()) != NULL) {
      if (f->sameRoom(*ch)) {
        int maxh = max(1, (int) f->hitLimit());
        int ratio = min(10, max(0, ((f->getHit() * 9) / maxh)));
        buf += format(VT_CURSPOS) % ch->getScreen() % 3;
        buf += format("%s<%s=%s>%s") % ch->purple() % fname(f->name) % prompt_mesg[ratio] % ch->norm();
        last.fighting = TRUE;

        ratio = fname(f->name).length() + strlen(prompt_mesg[ratio]);

        while (ratio < 25) {
          buf += " ";
          ratio++;
        }
      }
    } else {
      if (last.fighting) {
        buf += format(VT_CURSPOS) % ch->getScreen() % 3;
        buf += "                         ";
        last.fighting = FALSE;
      }
    }

    if (IS_SET(update, CHANGED_MUD)) {
      buf += format(VT_CURSPOS) % ch->getScreen() % 35;
      buf += format("%s") % GameTime::hmtAsString(GameTime::hourminTime());
    }

    if (IS_SET(update, CHANGED_TIME)) {
      time_t t1;
      struct tm *tptr;
      if ((t1 = time(0)) != -1) {
        tptr = localtime(&t1);

        // adjust time for users local site
        tptr->tm_hour += account->time_adjust;
        if (tptr->tm_hour < 0) {
          tptr->tm_hour += 24;
        } else if (tptr->tm_hour > 23)
          tptr->tm_hour -= 24;

        buf += format(VT_CURSPOS) % ch->getScreen() % 62;
        buf += format("%2d:%02d %2s") %
                ((!(tptr->tm_hour%12) ? 12 : tptr->tm_hour%12)) %
                tptr->tm_min %
                ((tptr->tm_hour >= 12) ? "PM" : "AM");
      }
    }

  } else {

    // Line 1:

    if (IS_SET(update, CHANGED_HP)) {
      buf += format(VT_CURSPOS) % (ch->getScreen() - 2) % 7;
      buf += format("%s%-5d ") % (current_hit > 2 ? VT_BOLDTEX : ANSI_RED) % ch->getHit();
      buf += ANSI_BLUE;

      for (i = 1; i <= current_hit; i++)
        buf += ANSI_BAR3;

      buf += ANSI_CYAN;

      for (i = 1; i <= missing_hit; i++)
        buf += ANSI_BAR3;
    }

    if (IS_SET(update, CHANGED_MANA) || IS_SET(update, CHANGED_PIETY) || IS_SET(update, CHANGED_LIFEFORCE)) {
      buf += format(VT_CURSPOS) % (ch->getScreen() - 2) % 34;

      if (ch->hasClass(CLASS_DEIKHAN) || ch->hasClass(CLASS_CLERIC))
        buf += format("%s%-5.1f ") % (current_mana ? VT_BOLDTEX : ANSI_RED) % ch->getPiety();
      else if (ch->hasClass(CLASS_SHAMAN))
        buf += format("%s%-5d ") % (current_mana ? VT_BOLDTEX : ANSI_RED) % ch->getLifeforce();
      else
        buf += format("%s%-5d ") % (current_mana ? VT_BOLDTEX : ANSI_RED) % ch->getMana();

      buf += ANSI_BLUE;

      for (i = 1; i <= current_mana; i++)
        buf += ANSI_BAR3;

      buf += ANSI_CYAN;

      for (i = 1; i <= missing_mana; i++)
        buf += ANSI_BAR3;
    }

    if (IS_SET(update, CHANGED_MOVE)) {
      buf += format(VT_CURSPOS) % (ch->getScreen() - 2) % 62;
      buf += format("%s%-5d ") % (current_moves ? VT_BOLDTEX : ANSI_RED) % ch->getMove();
      buf += ANSI_BLUE;

      for (i = 1; i <= current_moves; i++)
        buf += ANSI_BAR3;

      buf += ANSI_CYAN;

      for (i = 1; i <= missing_moves; i++)
        buf += ANSI_BAR3;
    }

    // Line 2:

    if ((f = ch->fight()) != NULL) {
      if (f->sameRoom(*ch)) {
        char StTemp[120];
        int ratio = min(10, max(0, ((f->getHit() * 9) / f->hitLimit())));

        memset(&StTemp, 0, sizeof(StTemp));
        snprintf(StTemp, cElements(StTemp), "<%s=%s>", fname(f->name).c_str(), prompt_mesg[ratio]);

        if (strlen(StTemp) > 22) {
          StTemp[19] = StTemp[20] = StTemp[21] = '.';
          StTemp[22] = '>';
          StTemp[23] = '\0';
        }

        for (int iRunner = strlen(StTemp); iRunner < 23; iRunner++)
          StTemp[iRunner] = ' ';

        StTemp[23] = '\0';

        buf += format(VT_CURSPOS) % (ch->getScreen() - 1) % 1;
        buf += format("%s%s%s") % ch->purple() % StTemp % ch->norm();

        last.fighting = TRUE;
      }
    } else {
      if (last.fighting) {
        buf += format(VT_CURSPOS) % (ch->getScreen() - 1) % 1;
        buf += "                      ";
        last.fighting = FALSE;
      }
    }

    if (IS_SET(update, CHANGED_EXP)) {
      buf += format(VT_CURSPOS) % (ch->getScreen() - 1) % 34;
      buf += format("%s%s") % ANSI_GREEN % ch->displayExp();
    }

    if (ch->isImmortal()) {
      if (IS_SET(update, CHANGED_ROOM)) {
        buf += format(VT_CURSPOS) % (ch->getScreen() - 1) % 62;
        buf += format("%s%-6d") % ANSI_GREEN % ch->roomp->number;
      }
    } else {
#if FACTIONS_IN_USE
      if (IS_SET(update, CHANGED_PERC)) {
        buf += format(VT_CURSPOS) % (ch->getScreen() - 1) % 45;
        buf += format("%s%3.4f") % ANSI_GREEN % ch->getPerc();
      }
#else
      if (ch->fight() && ch->awake() && ch->fight()->sameRoom(*ch)) {
        f = ch->fight()->fight();

        if (f && (f != ch) && ch->sameRoom(*f)) {
          char StTemp[120];
          int ratio = min(10, max(0, ((f->getHit() * 9) / f->hitLimit())));

          memset(&StTemp, 0, sizeof(StTemp));
          snprintf(StTemp, cElements(StTemp), "<%s=%s>", fname(f->name).c_str(), prompt_mesg[ratio]);

          if (strlen(StTemp) > 22) {
            StTemp[19] = StTemp[20] = StTemp[21] = '.';
            StTemp[22] = '>';
            StTemp[23] = '\0';
          }

          for (int iRunner = strlen(StTemp); iRunner < 23; iRunner++)
            StTemp[iRunner] = ' ';

          StTemp[23] = '\0';

          buf += format(VT_CURSPOS) % (ch->getScreen() - 1) % 55;
          buf += format("%s%s%s") % ch->purple() % StTemp % ch->norm();

          last.fighting = TRUE;
        } else {
          buf += format(VT_CURSPOS) % (ch->getScreen() - 1) % 55;
          buf += "                      ";
        }
      } else {
        buf += format(VT_CURSPOS) % (ch->getScreen() - 1) % 55;
        buf += "                      ";
      }
#endif
    }

    // Line 3:

    if (IS_SET(update, CHANGED_MUD)) {
      buf += format(VT_CURSPOS) % ch->getScreen() % 1;
      buf += format("   %8s   ") % GameTime::hmtAsString(GameTime::hourminTime());
    }

    time_t t1;
    struct tm *tptr;

    if ((t1 = time((time_t *) 0)) != -1) {
      tptr = localtime(&t1);

      // adjust time for users local site
      if (account)
        tptr->tm_hour += account->time_adjust;
      if (tptr->tm_hour < 0)
        tptr->tm_hour += 24;
      else if (tptr->tm_hour > 23)
        tptr->tm_hour -= 24;

      buf += format(VT_CURSPOS) % ch->getScreen() % 15;
      buf += format("   %2d:%02d %2s   ") %
            ((!(tptr->tm_hour%12) ? 12 : tptr->tm_hour%12)) %
            tptr->tm_min %
            ((tptr->tm_hour >= 12) ? "PM" : "AM");
    }

    if (update & CHANGED_EXP) {
      classIndT iClass;

      for (iClass = MAGE_LEVEL_IND; iClass < MAX_CLASSES; iClass++) {
        if (ch->getLevel(iClass)) {
          double iNeed = getExpClassLevel(iClass, ch->getLevel(iClass) + 1) - ch->getExp();

          buf += format(VT_CURSPOS) % ch->getScreen() % 34;

          if (ch->getLevel(iClass) >= MAX_MORT)
            buf += "0";
          else {
            char StTemp[20];

            memset(&StTemp, 0, sizeof(StTemp));

            snprintf(StTemp, cElements(StTemp), ((ch->getExp() < 100) ? "%.3f" : "%.0f"), iNeed);

            for (int iRunner = strlen(StTemp); iRunner < 11; iRunner++)
              StTemp[iRunner] = ' ';

            StTemp[11] = '\0';

            buf += StTemp;
          }

          break;
        }
      }
    }

    if (IS_SET(update, CHANGED_GOLD)) {
      buf += format(VT_CURSPOS) % ch->getScreen() % 62;
      buf += format("%s%-8d") % ANSI_GREEN % ch->getMoney();
    }

  }

  writeToQ(buf);
  writeToQ(VT_NORMALT);
  writeToQ(VT_CURREST);

  // Make sure it doesn't send a new '>' when automatically updated - Russ
  if ((update == CHANGED_TIME) || (update == CHANGED_MUD))
    prompt_mode = PROMPT_DONT_SEND;
}


// base class
sstring Comm::getComm(commTypeT comm){
  switch(comm){
    case TEXT:
      return getText();
      break;
    case CLIENT:
      return getClientText();
      break;
    case XML:
      return getXML();
      break;
  }

  return "";
}

// UncategorizedComm
UncategorizedComm::UncategorizedComm(const sstring &t){
  text=t;
}

sstring UncategorizedComm::getText(){
  return text;
}

sstring UncategorizedComm::getClientText(){
  return getText();
}

sstring UncategorizedComm::getXML(){
  return format("<uncategorized>%s</uncategorized>") % xmlescape(text);
}

// RoomExitComm
RoomExitComm::RoomExitComm(){
}


// WhoListComm
WhoListComm::WhoListComm(const sstring &w, bool o, int l, int i, bool ld,
                         const sstring &p, const sstring &t){
  who=w;
  online=o;
  level=l;
  idle=i;
  linkdead=ld;
  prof=p;
  title=t;
}

CmdMsgComm::CmdMsgComm(const sstring &c, const sstring &t){
  cmd=c;
  text=t;
}

sstring CmdMsgComm::getText(){
  return text;
}

sstring CmdMsgComm::getClientText(){
  return getText();
}

sstring CmdMsgComm::getXML(){
  return format("<cmdmsg cmd=\"%s\">%s</cmdmsg>") %
    xmlescape(cmd) % xmlescape(text);
}

TellFromComm::TellFromComm(const sstring &tt, const sstring &f,
                           const sstring &t, bool d, bool m){
  to=tt;
  from=f;
  text=t;
  drunk=d;
  mob=m;
}

TellToComm::TellToComm(const sstring &tt, const sstring &f, const sstring &t){
  to=tt;
  from=f;
  text=t;
}

SnoopComm::SnoopComm(const sstring &v, const sstring &t){
  vict=v;
  text=t;
}

LoginComm::LoginComm(const sstring &p, const sstring &t){
  prompt=p;
  text=t;
}

PromptComm::PromptComm(time_t t, int h, int m, float p, int l, int mv, int g, int r, const sstring &txt){
  time=t;
  hp=h;
  mana=h;
  piety=p;
  lifeforce=l;
  moves=mv;
  money=g;
  room=r;
  text=txt;
}

SoundComm::SoundComm(const sstring &st, const sstring &u, const sstring &s, const sstring &t, int v, int p, int r, int c){
  soundtype=st;
  url=u;
  text=s;
  type=t;
  volume=v;
  priority=p;
  repeats=r;
  cont=c;
}

const sstring xmlescape(const sstring &s) {
  sstring buf = "";

  boost::regex e("(^|[^<])<.>");
  boost::sregex_iterator m(s.begin(), s.end(), e);
  boost::sregex_iterator last_m;
  boost::sregex_iterator end;
  char code;

  for (; m != end; m++) {
    buf.append((*m).prefix());
    buf.append((*m)[1]);

    if ((*m)[0].str()[1] == '<')
      code=(*m)[0].str()[2];
    else
      code=(*m)[0].str()[1];

    switch(code){
      case 'h':
        buf.append(Config.mud_name());
        break;
      case 'H':
        buf.append(Config.mud_name_version());
        break;
      case 'R':
        buf.append(ANSI_RED_BOLD);
        break;
      case 'r':
        buf.append(ANSI_NORMAL);
        buf.append(ANSI_RED);
        break;
      case 'G':
        buf.append(ANSI_GREEN_BOLD);
        break;
      case 'g':
        buf.append(ANSI_NORMAL);
        buf.append(ANSI_GREEN);
        break;
      case 'y':
        buf.append(ANSI_ORANGE_BOLD);
        break;
      case 'Y':
        buf.append(ANSI_ORANGE_BOLD);
        break;
      case 'o':
        buf.append(ANSI_NORMAL);
        buf.append(ANSI_ORANGE);
        break;
      case 'O':
        buf.append(ANSI_NORMAL);
        buf.append(ANSI_ORANGE);
        break;
      case 'B':
        buf.append(ANSI_BLUE_BOLD);
        break;
      case 'b':
        buf.append(ANSI_NORMAL);
        buf.append(ANSI_BLUE);
        break;
      case 'P':
        buf.append(ANSI_PURPLE_BOLD);
        break;
      case 'p':
        buf.append(ANSI_NORMAL);
        buf.append(ANSI_PURPLE);
        break;
      case 'C':
        buf.append(ANSI_CYAN_BOLD);
        break;
      case 'c':
        buf.append(ANSI_NORMAL);
        buf.append(ANSI_CYAN);
        break;
      case 'W':
        buf.append(ANSI_WHITE_BOLD);
        break;
      case 'w':
        buf.append(ANSI_NORMAL);
        buf.append(ANSI_WHITE);
        break;
      case 'k':
        buf.append(VT_BOLDTEX);
        buf.append(ANSI_BLACK);
        break;
      case 'K':
        buf.append(ANSI_NORMAL);
        buf.append(ANSI_BLACK);
        break;
      case 'A':
        buf.append(VT_BOLDTEX);
        buf.append(ANSI_UNDER);
        break;
      case 'a':
        buf.append(ANSI_UNDER);
        break;
      case 'D':
        buf.append(VT_BOLDTEX);
        break;
      case 'd':
        buf.append(VT_BOLDTEX);
        break;
      case 'F':
        buf.append(ANSI_FLASH);
        break;
      case 'f':
        buf.append(ANSI_FLASH);
        break;
      case 'i':
        buf.append(VT_INVERTT);
        break;
      case 'I':
        buf.append(VT_INVERTT);
        break;
      case 'e':
        buf.append(ANSI_BK_ON_WH);
        break;
      case 'E':
        buf.append(ANSI_BK_ON_WH);
        break;
      case 'j':
        buf.append(ANSI_BK_ON_BK);
        break;
      case 'J':
        buf.append(ANSI_BK_ON_BK);
        break;
      case 'l':
        buf.append(ANSI_WH_ON_RD);
        break;
      case 'L':
        buf.append(ANSI_WH_ON_RD);
        break;
      case 'q':
        buf.append(ANSI_WH_ON_GR);
        break;
      case 'Q':
        buf.append(ANSI_WH_ON_GR);
        break;
      case 't':
        buf.append(ANSI_WH_ON_OR);
        break;
      case 'T':
        buf.append(ANSI_WH_ON_OR);
        break;
      case 'u':
        buf.append(ANSI_WH_ON_BL);
        break;
      case 'U':
        buf.append(ANSI_WH_ON_BL);
        break;
      case 'v':
        buf.append(ANSI_WH_ON_PR);
        break;
      case 'V':
        buf.append(ANSI_WH_ON_PR);
        break;
      case 'x':
        buf.append(ANSI_WH_ON_CY);
        break;
      case 'X':
        buf.append(ANSI_WH_ON_CY);
        break;
      case 'z':
        buf.append(ANSI_NORMAL);
        break;
      case 'Z':
        buf.append(ANSI_NORMAL);
        break;
      case '1':
        buf.append(ANSI_NORMAL);
        break;
    }
    last_m = m;
  }
  if (last_m != m)
    buf.append((*last_m).suffix());
  else
    buf = s;

  buf.inlineReplaceString("<<", "<");

  // escape for xml
  buf.inlineReplaceString("&", "&#38;");
  buf.inlineReplaceString("<", "&#60;");
  buf.inlineReplaceString(">", "&#62;");

  // ansi font styles
  buf.inlineReplaceString(VT_BOLDTEX, "<font style=\"bold\" />");
  buf.inlineReplaceString(ANSI_UNDER, "<font style=\"under\" />");
  buf.inlineReplaceString(VT_INVERTT, "<font style=\"invert\" />");
  buf.inlineReplaceString(ANSI_FLASH, "<font style=\"flash\" />");
  // ansi font colors
  buf.inlineReplaceString(ANSI_WHITE, "<font color=\"white\" />");
  buf.inlineReplaceString(ANSI_BLACK, "<font color=\"black\" />");
  buf.inlineReplaceString(ANSI_RED, "<font color=\"red\" />");
  buf.inlineReplaceString(ANSI_NORMAL, "<font color=\"norm\" />");
  buf.inlineReplaceString(ANSI_BLUE, "<font color=\"blue\" />");
  buf.inlineReplaceString(ANSI_CYAN, "<font color=\"cyan\" />");
  buf.inlineReplaceString(ANSI_GREEN, "<font color=\"green\" />");
  buf.inlineReplaceString(ANSI_ORANGE, "<font color=\"orange\" />");
  buf.inlineReplaceString(ANSI_PURPLE, "<font color=\"purple\" />");

  // colors with styles
  buf.inlineReplaceString(ANSI_RED_BOLD,
                          "<font style=\"bold\" color=\"red\" />");
  buf.inlineReplaceString(ANSI_GREEN_BOLD,
                          "<font style=\"bold\" color=\"green\" />");
  buf.inlineReplaceString(ANSI_ORANGE_BOLD,
                          "<font style=\"bold\" color=\"orange\" />");
  buf.inlineReplaceString(ANSI_YELLOW_BOLD,
                          "<font style=\"bold\" color=\"yellow\" />");
  buf.inlineReplaceString(ANSI_BLUE_BOLD,
                          "<font style=\"bold\" color=\"blue\" />");
  buf.inlineReplaceString(ANSI_PURPLE_BOLD,
                          "<font style=\"bold\" color=\"purple\" />");
  buf.inlineReplaceString(ANSI_CYAN_BOLD,
                          "<font style=\"bold\" color=\"cyan\" />");
  buf.inlineReplaceString(ANSI_WHITE_BOLD,
                          "<font style=\"bold\" color=\"white\" />");

  // colors with background
  buf.inlineReplaceString(ANSI_BK_ON_BK,
                          "<font bgcolor=\"black\" color=\"black\" />");
  buf.inlineReplaceString(ANSI_BK_ON_WH,
                          "<font bgcolor=\"white\" color=\"black\" />");
  buf.inlineReplaceString(ANSI_WH_ON_BL,
                          "<font bgcolor=\"blue\" color=\"white\" />");
  buf.inlineReplaceString(ANSI_WH_ON_CY,
                          "<font bgcolor=\"cyan\" color=\"white\" />");
  buf.inlineReplaceString(ANSI_WH_ON_GR,
                          "<font bgcolor=\"green\" color=\"white\" />");
  buf.inlineReplaceString(ANSI_WH_ON_OR,
                          "<font bgcolor=\"orange\" color=\"white\" />");
  buf.inlineReplaceString(ANSI_WH_ON_PR,
                          "<font bgcolor=\"purple\" color=\"white\" />");
  buf.inlineReplaceString(ANSI_WH_ON_RD,
                          "<font bgcolor=\"red\" color=\"white\" />");

  return buf;
}


