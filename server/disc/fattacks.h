#ifndef SERVER_DISC_FATTACKS_H_
#define SERVER_DISC_FATTACKS_H_

#include "misc/discipline.h"
#include "misc/skills.h"

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
