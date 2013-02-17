#ifndef SERVER_DISC_SOLDIERING_H_
#define SERVER_DISC_SOLDIERING_H_

// This is the WARRIOR SOLDIERING discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDSoldiering : public CDiscipline
{
public:
    CSkill skDoorbash;
    CSkill skDualWieldWarrior;
    CSkill skPowerMove;
    CSkill skDeathstroke;

    CDSoldiering();
    CDSoldiering(const CDSoldiering &a);
    CDSoldiering & operator=(const CDSoldiering &a);
    virtual ~CDSoldiering();
    virtual CDSoldiering * cloneMe() { return new CDSoldiering(*this); }

private:
};

#endif
