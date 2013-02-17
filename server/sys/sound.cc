#include "logging.h"
#include "misc/being.h"
#include "misc/low.h"
#include "misc/extern.h"
#include "sys/colorstring.h"
#include "misc/monster.h"

int RecGetObjRoom(const TThing *obj)
{
  if (obj->in_room != Room::NOWHERE)
    return(obj->in_room);
  if (dynamic_cast<TBeing *>(obj->parent))
    return(obj->parent->in_room);
  if (obj->equippedBy)
    return(obj->equippedBy->in_room);
  if (obj->parent)
    return(RecGetObjRoom(obj->parent));
  if (obj->riding)
    return(RecGetObjRoom(obj->riding));

  return Room::NOWHERE;
}

void MakeRoomNoise(TMonster *mob, int room, const char *local_snd, const char *distant_snd)
{
  dirTypeT door;
  TBeing *ch = NULL;
  TRoom *rp, *orp;
  TThing *t=NULL;
  char buf[256];

  if ((rp = real_roomp(room)) && local_snd && *local_snd) {
    for(StuffIter it=rp->stuff.begin();it!=rp->stuff.end() && (t=*it);++it) {
      ch = dynamic_cast<TBeing *>(t);
      if (!ch || !ch->desc) {
        continue;
      }
      if (ch->awake()) {
        sprintf(buf, "%s", addNameToBuf(ch, ch->desc, mob, local_snd, COLOR_MOBS).c_str());
        ch->sendTo(COLOR_BASIC, buf);
      }
    }
  }

  if (rp && distant_snd) {
    for (door = MIN_DIR; door < MAX_DIR; door++) {
      if (rp->dir_option[door] && (orp = real_roomp(rp->dir_option[door]->to_room))) {
        for(StuffIter it=orp->stuff.begin();it!=orp->stuff.end() && (t=*it);++it) {
          ch = dynamic_cast<TBeing *>(t);
          if (!ch) {
            continue;
          }
          if (ch->desc && !IS_SET(ch->desc->autobits, AUTO_NOSHOUT) && ch->awake()) {
            sprintf(buf, "%s", addNameToBuf(ch, ch->desc, mob, distant_snd, COLOR_MOBS).c_str());
//            sprintf(buf, "%s", colorString(ch, ch->desc, distant_snd, NULL, COLOR_BASIC, TRUE));
            ch->sendTo(COLOR_BASIC, buf);
          }
        }
      }
    }
  }
}

void MakeNoise(int room, const char *local_snd, const char *distant_snd)
{
  dirTypeT door;
  TBeing *ch = NULL;
  TRoom *rp = NULL, *orp = NULL;
  TThing *t = NULL;

  if ((rp = real_roomp(room))) {
    for(StuffIter it=rp->stuff.begin();it!=rp->stuff.end() && (t=*it);++it) {
      ch = dynamic_cast<TBeing *>(t);
      if (!ch) {
        continue;
      }
      if (ch->awake()) {
        ch->sendTo(COLOR_BASIC, local_snd);
      }
    }
  }
  if (!rp) {
    vlogf(LOG_MISC, format("Testing log: No rp in MakeNoise for %s") %  ((ch->name) ? ch->name : "null"));
    return;
  }
  for (door = MIN_DIR; door < MAX_DIR; door++) {
    if (rp->dir_option[door] && (orp = real_roomp(rp->dir_option[door]->to_room))) {
      for(StuffIter it=orp->stuff.begin();it!=orp->stuff.end() && (t=*it);++it) {
        ch = dynamic_cast<TBeing *>(t);
        if (!ch) {
          continue;
        }
        if (ch->desc && !IS_SET(ch->desc->autobits, AUTO_NOSHOUT) && ch->awake()) {
          ch->sendTo(COLOR_BASIC, distant_snd);
        }
      }
    }
  }
}

int noise(const TBeing *ch)
{
  int i, total = 0;
  affectedData *af;

  for (i = MIN_WEAR; i < MAX_WEAR; i++) {
    if ((i == WEAR_FINGER_L) || (i == WEAR_FINGER_R) ||
        (i == HOLD_RIGHT) || (i == HOLD_LEFT))
      continue;

    TObj *tobj = dynamic_cast<TObj *>(ch->equipment[i]);
    if (tobj)
      total += tobj->itemNoise();
  }
  if (ch->isAffected(AFF_SNEAK) && !ch->fight())
    total -= 1 + ch->GetMaxLevel() / 2;

  // if in natural environs, reduce even further
  // traps areas that SHOULDN'T aid them
  if (ch->getRace() == RACE_ELVEN) {
    switch (ch->roomp->getSectorType()) {
      case SECT_ARCTIC_CITY:
      case SECT_ARCTIC_ROAD:
      case SECT_ARCTIC_BUILDING:
      case SECT_TEMPERATE_CITY:
      case SECT_TEMPERATE_ROAD:
      case SECT_TEMPERATE_BUILDING:
      case SECT_TROPICAL_CITY:
      case SECT_TROPICAL_ROAD:
      case SECT_TROPICAL_BUILDING:
        break;
      default:
        total -= 20;
      break;
    }
  }

  // adjust for height, ogres = noisy, hobbits quiet
  total += ch->getHeight() - 70;

  // big, strong, loud and arrogant
  if ((ch->getRace() == RACE_OGRE) || (ch->getRace() == RACE_DWARF))
    total += 20;

  for (af = ch->affected; af; af = af->next) {
    if (af->location == APPLY_NOISE) {
      total += af->modifier;
    }
  }
  return (total);
}

int TBeing::applySoundproof() const
{
  if (checkSoundproof() && !isImmortal()) {
    sendTo("You are in a silence zone, you can't make a sound!\n\r");
    return TRUE;                /* for shouts, emotes, etc */
  }
  return FALSE;
}

int TThing::checkSoundproof() const
{
  return (roomp->isRoomFlag(ROOM_SILENCE));
}
