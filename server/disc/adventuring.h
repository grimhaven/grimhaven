#ifndef __DISC_ADVENTURING_H
#define __DISC_ADVENTURING_H

// This contains the general knowledge areas that _every_ pc knows.  All of
// these are "learn by mistake".  None can be practiced.
// On the same token, I don know know if any of these set any character flags.
// They don't seem to directly have any functions.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDAdventuring : public CDiscipline
{
  public:
    CSkill skFishing;
    CSkill skLogging;
    CSkill skAlcoholism;
    CSkill skRide;
    CSkill skSwim;
    CSkill skClimb;
    CSkill skSign;
    CSkill skKnowPeople;
    CSkill skKnowGiant;
    CSkill skKnowVeggie;
    CSkill skKnowAnimal;
    CSkill skKnowReptile;
    CSkill skKnowUndead;
    CSkill skKnowOther;
    CSkill skKnowDemon;
    CSkill skReadMagic;
    CSkill skBandage;
    CSkill skFastHeal;
    CSkill skEvaluate;
    CSkill skTactics;
    CSkill skDissect;
    CSkill skOffense;
    CSkill skDefense;
    CSkill skGenWeapons;
    CSkill skWhittle;
    CSkill skMend;
    CSkill skButcher;
    CSkill skGutterCant;
    CSkill skGnollJargon;
    CSkill skTroglodytePidgin;

    CDAdventuring()
      : CDiscipline(),
        skFishing(),
        skLogging(),
        skAlcoholism(),
        skRide(),
        skSwim(),
        skClimb(),
        skSign(),
        skKnowPeople(),
        skKnowGiant(),
        skKnowVeggie(),
        skKnowAnimal(),
        skKnowReptile(),
        skKnowUndead(),
        skKnowOther(),
        skKnowDemon(),
        skReadMagic(),
        skBandage(),
        skFastHeal(),
        skEvaluate(),
        skTactics(),
        skDissect(),
        skOffense(),
        skDefense(),
        skGenWeapons(),
        skWhittle(),
        skMend(),
        skButcher(),
        skGutterCant(),
        skGnollJargon(),
        skTroglodytePidgin()
    {
    }
    CDAdventuring(const CDAdventuring &a)
      : CDiscipline(a),
        skFishing(a.skFishing),
        skLogging(a.skLogging),
        skAlcoholism(a.skAlcoholism),
        skRide(a.skRide),
        skSwim(a.skSwim),
        skClimb(a.skClimb),
        skSign(a.skSign),
        skKnowPeople(a.skKnowPeople),
        skKnowGiant(a.skKnowGiant),
        skKnowVeggie(a.skKnowVeggie),
        skKnowAnimal(a.skKnowAnimal),
        skKnowReptile(a.skKnowReptile),
        skKnowUndead(a.skKnowUndead),
        skKnowOther(a.skKnowOther),
        skKnowDemon(a.skKnowDemon),
        skReadMagic(a.skReadMagic),
        skBandage(a.skBandage),
        skFastHeal(a.skFastHeal),
        skEvaluate(a.skEvaluate),
        skTactics(a.skTactics),
        skDissect(a.skDissect),
        skOffense(a.skOffense),
        skDefense(a.skDefense),
        skGenWeapons(a.skGenWeapons),
        skWhittle(a.skWhittle),
        skMend(a.skMend),
        skButcher(a.skButcher),
        skGutterCant(a.skGutterCant),
        skGnollJargon(a.skGnollJargon),
        skTroglodytePidgin(a.skTroglodytePidgin)
    {
    }
    CDAdventuring & operator=(const CDAdventuring &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skFishing = a.skFishing;
      skLogging = a.skLogging;
      skAlcoholism = a.skAlcoholism;
      skRide = a.skRide;
      skSwim = a.skSwim;
      skClimb = a.skClimb;
      skSign = a.skSign;
      skKnowPeople = a.skKnowPeople;
      skKnowGiant = a.skKnowGiant;
      skKnowVeggie = a.skKnowVeggie;
      skKnowAnimal = a.skKnowAnimal;
      skKnowReptile = a.skKnowReptile;
      skKnowUndead = a.skKnowUndead;
      skKnowOther = a.skKnowOther;
      skKnowDemon = a.skKnowDemon;
      skReadMagic = a.skReadMagic;
      skBandage = a.skBandage;
      skFastHeal = a.skFastHeal;
      skEvaluate = a.skEvaluate;
      skTactics = a.skTactics;
      skDissect = a.skDissect;
      skOffense = a.skOffense;
      skDefense = a.skDefense;
      skGenWeapons = a.skGenWeapons;
      skWhittle = a.skWhittle;
      skMend = a.skMend;
      skButcher = a.skButcher;
      skGutterCant = a.skGutterCant;
      skGnollJargon = a.skGnollJargon;
      skTroglodytePidgin = a.skTroglodytePidgin;

      return *this;
    }
    virtual ~CDAdventuring() {};

    virtual CDAdventuring * cloneMe() {
      return new CDAdventuring(*this);
    }

    bool isAutomatic(){ return true; }

private:
};

int dissect(TBeing *, TObj *);

#endif
