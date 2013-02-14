#ifndef __DISC_RANGED_H
#define __DISC_RANGED_H

// This is the RANGED discipline.

#include "discipline.h"
#include "skills.h"

class CDRanged : public CDiscipline
{
public:
    CSkill skRangedSpec;
    CSkill skFastLoad;

    CDRanged() :
      CDiscipline(),
      skRangedSpec(),
      skFastLoad() {
    }
    CDRanged(const CDRanged &a) :
      CDiscipline(a),
      skRangedSpec(a.skRangedSpec),
      skFastLoad(a.skFastLoad) {
    }
    CDRanged & operator=(const CDRanged &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skRangedSpec = a.skRangedSpec;
      skFastLoad = a.skFastLoad;
      return *this;
    }
    virtual ~CDRanged() {}
    virtual CDRanged * cloneMe() { return new CDRanged(*this); }

private:
};

#endif

