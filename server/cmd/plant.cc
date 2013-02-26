#include "core/logging.h"
#include "util/math.h"

#include "sys/handler.h"
#include "misc/room.h"
#include "misc/being.h"
#include "misc/monster.h"
#include "obj/tool.h"
#include "obj/plant.h"

int seed_plant(TBeing *, TObj *);
int thief_plant(TBeing *, TObj *, sstring);

int TBeing::doPlant(sstring arg) {
  sstring object = arg.word(0);
  sstring victim = arg.word(1);

  if (object.empty()) {
    sendTo("Plant what?\n\r");
    return false;
  }

  TObj *item;
  if (!(item = generic_find_obj(object, FIND_OBJ_INV|FIND_OBJ_HELD, this))){
    sendTo("You don't have that item.\n\r");
    return false;
  }

  if (victim.empty())
    return seed_plant(this, item);
  else
    return thief_plant(this, item, victim);
}

bool thief_can_plant(TBeing *thief, TBeing *victim) {
  if (thief->hasWizPower(POWER_WIZARD)) {
    if (victim->isImmortal()) {
      thief->sendTo("That wouldn't be very nice!\n\r");
      return false;
    }
    return true;
  }

  if (!thief->doesKnowSkill(SKILL_STEAL)) {
    thief->sendTo("You know nothing about planting items.\n\r");
    return false;
  }

  if (victim->isImmortal()) {
    thief->sendTo("An immortal would see right through that.\n\r");
    return false;
  }

  if ((thief->equipment[HOLD_LEFT] || thief->equipment[HOLD_RIGHT])) {
      thief->sendTo("Your hands must be empty to plant something.\n\r");
      return false;
  }

  if (thief->checkPeacefulAction("plant anything"))
    return false;

  if (thief->roomp->isRoomFlag(ROOM_NO_STEAL)) {
    thief->sendTo("Such actions are prevented here.\n\r");
    return false;
  }

  if (victim == thief) {
    thief->sendTo("How could you plant something on yourself without noticing?\n\r");
    return false;
  }

  if (thief->riding) {
    thief->sendTo("The fact that you're mounted makes you a bit conspicuous for that.\n\r");
    return false;
  }

  if (thief->isFlying()) {
    thief->sendTo("The fact that you're flying makes you a bit conspicuous for that.\n\r");
    return false;
  }

  if (victim->isShopkeeper()) {
    thief->sendTo("Oh, bad move.  Bad move.\n\r");
    vlogf(LOG_CHEAT, format("%s just tried to plant on a shopkeeper! [%s]") %
          thief->getName() % victim->getName());
    return false;
  }

  return true;
}

int thief_plant_chance(TBeing *thief, TBeing *victim) {
  int victim_level = victim->GetMaxLevel();
  int level = thief->getSkillLevel(SKILL_PLANT);
  int modifier = (level - victim_level) / 3;

  modifier += thief->plotStat(STAT_CURRENT, STAT_DEX, -70, 15, 0);

  if (!victim->awake())
    modifier += 50;

  if (victim_level > level ||
      victim->isLucky(thief->spellLuckModifier(SKILL_PLANT)))
    modifier -= 45;

  modifier += victim->getCond(DRUNK) / 4;

  if (!victim->isPc())
    modifier -= dynamic_cast<TMonster *>(victim)->susp() / 2;

  int competence = thief->getSkillValue(SKILL_PLANT);
  modifier = max(min(modifier, 100 - competence), -100);
  return competence + modifier;
}


int thief_plant(TBeing *thief, TObj *item, sstring victim_arg) {
  TBeing *victim;

  if (!(victim = get_char_room_vis(thief, victim_arg))){
    thief->sendTo("You don't see that person.\n\r");
    return false;
  }

  if (!thief_can_plant(thief, victim))
    return false;

  int rc;
  if (thief->bSuccess(thief_plant_chance(thief, victim), SKILL_PLANT)) {
    rc = thief->doGive(victim, item, GIVE_FLAG_SILENT_VICT);
    thief->sendTo("You escaped detection.\n\r");
  } else {
    rc = thief->doGive(victim, item);
    victim->sendTo("That seemed suspicious.\n\r");
    thief->sendTo("You were detected.\n\r");
  }
  return rc;
}

int seed_plant(TBeing *ch, TObj *item){
  TTool *seeds = dynamic_cast<TTool *>(item);
  if (!seeds || seeds->getToolType() != TOOL_SEED) {
    ch->sendTo("You need to specify some seeds to plant.\n\r");
    return false;
  }

  TRoom *room = ch->roomp;
  if(room->isFallSector() || room->isWaterSector() ||
     room->isIndoorSector() || room->isUnderwaterSector()){
    ch->sendTo("You can't plant anything here.\n\r");
    return false;
  }

  int existing = 0;
  for (auto thing : room->stuff) {
    if (dynamic_cast<TPlant *>(thing) && ++existing == 8) {
      ch->sendTo("There isn't any room for more plants here.\n\r");
      return false;
    }
  }

  ch->sendTo("You begin to plant some seeds.\n\r");
  start_task(ch, seeds, NULL, TASK_PLANT, "", 2, ch->inRoom(), 0, 0, 5);
  return true;
}

// vim: ft=cpp:tw=79:sw=2:sts=2:ts=8:et
