#ifndef __DISC_THIEF_FIGHT_H
#define __DISC_THIEF_FIGHT_H

// This is the THIEF FIGHTHING skills discipline.

#include "discipline.h"
#include "skills.h"

class CDThiefFight : public CDiscipline
{
public:
    CSkill skDodgeThief;
    CSkill skDualWieldThief;

    CDThiefFight()
      : CDiscipline(),
      skDodgeThief(),
      skDualWieldThief() {
    }
    CDThiefFight(const CDThiefFight &a)
      : CDiscipline(a),
      skDodgeThief(a.skDodgeThief),
      skDualWieldThief(a.skDualWieldThief) {
    }
    CDThiefFight & operator=(const CDThiefFight &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skDodgeThief = a.skDodgeThief;
      skDualWieldThief = a.skDualWieldThief;
      return *this;
    }
    virtual ~CDThiefFight() {}
    virtual CDThiefFight * cloneMe() { return new CDThiefFight(*this); }
private:
};

#endif
