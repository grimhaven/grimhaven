#ifndef __DISC_POISONS_H
#define __DISC_POISONS_H

// This is the THIEF POISONS discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDPoisons : public CDiscipline
{
public:
    CSkill skPoisonWeapons;

    CDPoisons()
      : CDiscipline(),
      skPoisonWeapons() {
    }
    CDPoisons(const CDPoisons &a)
      : CDiscipline(a),
      skPoisonWeapons(a.skPoisonWeapons) {
    }
    CDPoisons & operator=(const CDPoisons &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skPoisonWeapons = a.skPoisonWeapons;
      return *this;
    }
    virtual ~CDPoisons() {}
    virtual CDPoisons * cloneMe() { return new CDPoisons(*this); }
private:
};


#endif
