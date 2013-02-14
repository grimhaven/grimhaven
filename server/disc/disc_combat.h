#ifndef __DISC_COMBAT_H
#define __DISC_COMBAT_H   1

// This contains the general combat skills.

#include "discipline.h"
#include "skills.h"

class CDCombat : public CDiscipline
{
  public:
    CSkill skBarehand;
    CSkill skArmorUse;
    CSkill skSlash;
    CSkill skBow;
    CSkill skPierce;
    CSkill skBlunt;
    CSkill skSharpen;
    CSkill skDull;

    CDCombat();
    CDCombat(const CDCombat &a);
    CDCombat & operator=(const CDCombat &a);
    virtual ~CDCombat();
    virtual CDCombat * cloneMe() { return new CDCombat(*this); }

    bool isBasic(){ return true; }

private:
};

    void sharpen(TBeing *, TThing *);
    void dull(TBeing *, TThing *);

#endif

