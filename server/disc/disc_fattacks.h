#ifndef __DISC_FOCUSED_ATTACKS_H
#define __DISC_FOCUSED_ATTACKS_H

#include "discipline.h"
#include "skills.h"

class CDFAttacks : public CDiscipline
{
public:
    CSkill skQuiveringPalm;
    CSkill skCriticalHitting;

    CDFAttacks();
    CDFAttacks(const CDFAttacks &a);
    CDFAttacks & operator=(const CDFAttacks &a);
    virtual ~CDFAttacks();
    virtual CDFAttacks * cloneMe();

private:
};

#endif







