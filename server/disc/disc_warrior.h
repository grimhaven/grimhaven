#ifndef __DISC_WARRIOR_H
#define __DISC_WARRIOR_H

// This is the WARRIOR discipline.

#include "discipline.h"
#include "skills.h"

class CDWarrior : public CDiscipline
{
public:
// Level 2
    CSkill skKick;
//Level 10
    CSkill skBash;
//Level 12
    CSkill skHeadbutt;

//Level 15
    CSkill skRescue;

//Level 20
    CSkill skBlacksmithing;
    CSkill skDisarm;
//Level 25
    CSkill skBerserk;
    CSkill skSwitch;

    CSkill skKneestrike;
    CSkill skTrip;
    CDWarrior()
      : CDiscipline(),
      skKick(),
      skBash(),
      skHeadbutt(),
      skRescue(),
      skBlacksmithing(),
      skDisarm(),
      skBerserk(),
      skSwitch(),
      skKneestrike(),
      skTrip() {
    }
    CDWarrior(const CDWarrior &a)
      : CDiscipline(a),
      skKick(a.skKick),
      skBash(a.skBash),
      skHeadbutt(a.skHeadbutt),
      skRescue(a.skRescue),
      skBlacksmithing(a.skBlacksmithing),
      skDisarm(a.skDisarm),
      skBerserk(a.skBerserk),
      skSwitch(a.skSwitch),
      skKneestrike(a.skKneestrike),
      skTrip(a.skTrip) {
    }
    CDWarrior & operator=(const CDWarrior &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skKick = a.skKick;
      skBash = a.skBash;
      skHeadbutt = a.skHeadbutt;
      skRescue = a.skRescue;
      skBlacksmithing = a.skBlacksmithing;
      skDisarm = a.skDisarm;
      skBerserk = a.skBerserk;
      skSwitch = a.skSwitch;
      skKneestrike = a.skKneestrike;
      skTrip = a.skTrip;
      return *this;
    }
    virtual ~CDWarrior() {}
    virtual CDWarrior * cloneMe() { return new CDWarrior(*this); }

    bool isBasic(){ return true; }

private:
};

    int berserk(TBeing *);
    void repair(TBeing *, TObj *);

#endif

