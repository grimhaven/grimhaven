#include "misc/being.h" // TBeing
#include "misc/extern.h" // combatRound
#include "sys/handler.h" // get_char_room_vis

int vampire_bite_hits(TBeing *a, TBeing *v) {
  act("You sink your fangs deep into $N's neck and suck $S <r>blood<1>!",
      FALSE, a, NULL, v, TO_CHAR);
  act("$n sinks $s fangs deep into $N's neck and sucks $S <r>blood<1>!",
      FALSE, a, NULL, v, TO_NOTVICT);
  act("$n sinks $s fangs deep into your neck and sucks your <r>blood<1>!",
      FALSE, a, NULL, v, TO_VICT);

  int rc = a->reconcileDamage(v, v->getHit()+5, DAMAGE_DRAIN);
  v->setHit(-5); // sometimes the above doesn't set to -5 properly

  a->gainCondition(FULL, 15);
  a->gainCondition(THIRST, 15);
  act("You feel satiated.", FALSE, a, NULL, v, TO_CHAR);

  act("You reel about unsteadily, flush with <r>blood<1>.",
      FALSE, a, NULL, v, TO_CHAR);

  // stop the fight? - Beorat
  if (a->fight()) {
    a->stopFighting();
    v->stopFighting();
  }

  if (v->isPc() && !v->isVampire() && !v->hasQuestBit(TOG_VAMPIRE) &&
      !v->hasQuestBit(TOG_BITTEN_BY_VAMPIRE)) {
    affectedData aff;
    aff.type = AFFECT_BITTEN_BY_VAMPIRE;
    aff.location = APPLY_NONE;
    aff.duration = 24 * Pulse::UPDATES_PER_MUDHOUR;

    v->affectTo(&aff);
  }

  a->addToWait(combatRound(5));

  return rc;
}

int vampire_bite_misses(TBeing *a, TBeing *v) {
  act("You try to bite $N's neck but $E fights you off!",
      FALSE, a, NULL, v, TO_CHAR);
  act("$n tries to bite $N's neck, but $M fights $m off!",
      FALSE, a, NULL, v, TO_NOTVICT);
  act("$n tries to bite your neck, but you fight $M off!",
      FALSE, a, NULL, v, TO_VICT);
  return TRUE;
}

int vampire_bite(TBeing *a, TBeing *v) {
  if (v->isUndead() || v->isColdBlooded()) {
    a->sendTo("You can only do this to living, warm-blooded opponents.\n\r");
    return FALSE;
  }

  if (v->getPosition() <= POSITION_INCAP){
    a->sendTo("That victim is too close to death already.\n\r");
    return FALSE;
  }

  a->reconcileDamage(v, 0, DAMAGE_DRAIN);

  // wtf is this craziness - Beorat
  if (a->isImmortal() ||
        (
          // more total hitpoints than target
          v->hitLimit() < a->hitLimit() &&
          // and
          (
            // more than 10 levels above target
            a->GetMaxLevel() > v->GetMaxLevel() + 10 ||
            // or
            (
              // l10 or lower dumb animal with less than 25% hit points that we
              // are of higher level than
              a->GetMaxLevel() > v->GetMaxLevel() &&
              v->isDumbAnimal() &&
              v->GetMaxLevel() <= 10 &&
              v->getHit() < (int)(v->hitLimit()/4.0)
            )
          ) &&
          // and we hit
          a->hits(v, a->attackRound(v) - v->defendRound(a))
        )
      ) {
    return vampire_bite_hits(a, v);
  }

  return vampire_bite_misses(a, v);
}

int lycanthrope_bite_hits(TBeing *a, TBeing *v) {
    act("You sink your teeth into $N's flesh and savage $M viciously!",
        FALSE, a, NULL, v, TO_CHAR);
    act("$n sinks $s teeth into $N's flesh and savages $M viciously!",
        FALSE, a, NULL, v, TO_NOTVICT);
    act("$n sinks $s teeth into your flesh and savages you viciously!",
        FALSE, a, NULL, v, TO_VICT);

    int rc = a->reconcileDamage(v, ::number(5,25), DAMAGE_STOMACH_WOUND);

    if (v->isPc() && !v->isLycanthrope()) {
      act("You feel a burning in your <r>blood<1>.",
          FALSE, a, NULL, v, TO_VICT);
      v->setQuestBit(TOG_LYCANTHROPE);
    }

    a->addToWait(combatRound(3));

    return rc;
}

int lycanthrope_bite_misses(TBeing *a, TBeing *v) {
  act("You try to bite $N but $E fights you off!",
      FALSE, a, NULL, v, TO_CHAR);
  act("$n tries to bite $N, but $M fights $m off!",
      FALSE, a, NULL, v, TO_NOTVICT);
  act("$n tries to bite you, but you fight $M off!",
      FALSE, a, NULL, v, TO_VICT);
  return TRUE;
}

int lycanthrope_bite(TBeing *a, TBeing *v) {
  a->reconcileDamage(v, 0, DAMAGE_STOMACH_WOUND);

  if (a->hits(v, a->attackRound(v) - v->defendRound(a))) {
    return lycanthrope_bite_hits(a, v);
  }
  return lycanthrope_bite_misses(a, v);
}

int TBeing::doBite(const sstring &arg) {
  TBeing *victim;

  if (!isVampire() && !getMyRace()->isLycanthrope())
    return doAction(arg, CMD_BITE);

  if (!roomp)
    return FALSE;

  if (arg.empty()) {
    sendTo(isVampire() ?
        "Whose blood do you wish to suck?\n\r" :
        "Who do you want to bite?\n\r");
    return FALSE;
  }

  if (!((victim = get_char_room_vis(this, arg)) || (victim = fight())))
    // fall back to generic action bite (ie biting an object)
    return doAction(arg, CMD_BITE);

  if (!sameRoom(*victim)) {
    sendTo("That person isn't around.\n\r");
    return FALSE;
  }

  if (victim == this) {
    sendTo(isVampire() ?
        "Sucking blood from yourself would not be effective.\n\r" :
        "Biting yourself would not be wise.\n\r");
    return FALSE;
  }

  if (checkPeaceful())
    return FALSE;

  if (noHarmCheck(victim))
    return FALSE;

  if(victim->isImmortal()){
    sendTo("Biting an immortal would be unwise.\n\r");
    return FALSE;
  }

  if (isVampire())
    return vampire_bite(this, victim);
  return lycanthrope_bite(this, victim);
}

// vim: ft=cpp:tw=79:sw=2:sts=2:ts=8:et
