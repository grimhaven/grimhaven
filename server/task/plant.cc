#include "core/logging.h"
#include "misc/room.h"
#include "misc/being.h"
#include "misc/low.h"
#include "obj/tool.h"
#include "obj/plant.h"

void task_plant_continue(TTask *task, TBeing *ch, TTool *tool) {
  switch (task->timeLeft) {
    case 2:
      act("You dig a little hole for some seeds from $p.",
          false, ch, tool, 0, TO_CHAR);
      act("$n digs a little hole.",
          true, ch, tool, 0, TO_ROOM);
      break;
    case 1:
      act("You put some seeds from $p into your hole.",
          false, ch, tool, 0, TO_CHAR);
      act("$n puts some seeds from $p into the hole.",
          true, ch, tool, 0, TO_ROOM);
      tool->addToToolUses(-1);
      break;
    case 0:
      act("You cover up the hole.",
          false, ch, tool, 0, TO_CHAR);
      act("$n covers up the hole.",
          true, ch, tool, 0, TO_ROOM);
      break;
  }
  task->timeLeft--;
}

void task_plant_finish(TBeing *ch, TTool *tool) {
  act("You finish planting $p.",
      false, ch, tool, 0, TO_CHAR);
  act("$n finishes planting $p.",
      true, ch, tool, 0, TO_ROOM);
  ch->stopTask();

  TPlant *plant = dynamic_cast<TPlant *>(read_object(Obj::GENERIC_PLANT, VIRTUAL));
  if (plant) {
    plant->setType(seed_to_plant(tool->objVnum()));
    plant->updateDesc();
    *ch->roomp += *plant;
  } else {
    vlogf(LOG_BUG, "failed to get a plant obj when planting");
  }

  if (tool->getToolUses() <= 0) {
    act("You discard $p because it is empty.",
        false, ch, tool, 0, TO_CHAR);
    delete tool;
  }
}

void task_plant_stop(TBeing *ch) {
  act("You stop planting your seeds.",
      false, ch, 0, 0, TO_CHAR);
  act("$n stops planting seeds.",
      true, ch, 0, 0, TO_ROOM);
  ch->stopTask();
}

int task_plant(TBeing *ch, cmdTypeT cmd, const char *, int pulse, TRoom *, TObj *obj) {
  if (ch->utilityTaskCommand(cmd) || ch->nobrainerTaskCommand(cmd))
    return false;

  TTask *task = ch->task;

  // basic tasky safechecking
  if (ch->isLinkdead() || (ch->in_room != task->wasInRoom) || !obj) {
    task_plant_stop(ch);
    return false; // returning false lets command be interpreted
  }

  TTool *tool = dynamic_cast<TTool *>(obj);

  if (task->timeLeft < 0) {
    task_plant_finish(ch, tool);
    return false;
  }

  switch (cmd) {
    case CMD_TASK_CONTINUE:
      task->calcNextUpdate(pulse, Pulse::MOBACT * 3);
      task_plant_continue(task, ch, tool);
      break;
    case CMD_ABORT:
    case CMD_STOP:
      task_plant_stop(ch);
      break;
    case CMD_TASK_FIGHTING:
      ch->sendTo("You can't properly plant seeds while under attack.\n\r");
      ch->stopTask();
      break;
    default:
      if (cmd < MAX_CMD_LIST)
        warn_busy(ch);
      break;                    // eat the command
  }

  return true;
}

// vim: ft=cpp:tw=79:sw=2:sts=2:ts=8:et
