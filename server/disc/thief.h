#ifndef SERVER_DISC_THIEF_H_
#define SERVER_DISC_THIEF_H_

// This is the BASIC THIEF discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDThief : public CDiscipline
{
public:
    CSkill skSwindle;
    CSkill skSneak;
    CSkill skStabbing;
    CSkill skRetreatThief;
    CSkill skKickThief;
    CSkill skPickLock;
    CSkill skBackstab;
    CSkill skSearch;
    CSkill skSpy;
    CSkill skSwitchThief;
    CSkill skSteal;
    CSkill skDetectTraps;
    CSkill skSubterfuge;
    CSkill skDisarmTraps;
    CSkill skCudgel;
    CSkill skHide;
    CSkill skDisarmThief;
    CSkill skTrack;

    CSkill skRepairThief;

    CDThief()
      : CDiscipline(),
      skSwindle(),
      skSneak(),
      skStabbing(),
      skRetreatThief(),
      skKickThief(),
      skPickLock(),
      skBackstab(),
      skSearch(),
      skSpy(),
      skSwitchThief(),
      skSteal(),
      skDetectTraps(),
      skSubterfuge(),
      skDisarmTraps(),
      skCudgel(),
      skHide(),
      skDisarmThief(),
      skTrack(),
      skRepairThief() {
    }
    CDThief(const CDThief &a)
      : CDiscipline(a),
      skSwindle(a.skSwindle),
      skSneak(a.skSneak),
      skStabbing(a.skStabbing),
      skRetreatThief(a.skRetreatThief),
      skKickThief(a.skKickThief),
      skPickLock(a.skPickLock),
      skBackstab(a.skBackstab),
      skSearch(a.skSearch),
      skSpy(a.skSpy),
      skSwitchThief(a.skSwitchThief),
      skSteal(a.skSteal),
      skDetectTraps(a.skDetectTraps),
      skSubterfuge(a.skSubterfuge),
      skDisarmTraps(a.skDisarmTraps),
      skCudgel(a.skCudgel),
      skHide(a.skHide),
      skDisarmThief(a.skDisarmThief),
      skTrack(a.skTrack),
      skRepairThief(a.skRepairThief) {
    }
    CDThief & operator=(const CDThief &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skSwindle = a.skSwindle;
      skSneak = a.skSneak;
      skStabbing = a.skStabbing;
      skRetreatThief = a.skRetreatThief;
      skKickThief = a.skKickThief;
      skPickLock = a.skPickLock;
      skBackstab = a.skBackstab;
      skSearch = a.skSearch;
      skSpy = a.skSpy;
      skSwitchThief = a.skSwitchThief;
      skSteal = a.skSteal;
      skDetectTraps = a.skDetectTraps;
      skSubterfuge = a.skSubterfuge;
      skDisarmTraps = a.skDisarmTraps;
      skCudgel = a.skCudgel;
      skHide = a.skHide;
      skDisarmThief = a.skDisarmThief;
      skTrack = a.skTrack;
      skRepairThief = a.skRepairThief;
      return *this;
    }
    virtual ~CDThief() {}
    virtual CDThief * cloneMe() { return new CDThief(*this); }

    bool isBasic(){ return true; }

private:
};

int sneak(TBeing *, spellNumT);
int subterfuge(TBeing *, TBeing *);
int pickLocks(TBeing *, const char *, const char *, const char *);
int spy(TBeing *);
int hide(TBeing *, spellNumT);
int disguise(TBeing *, char *);

#endif
