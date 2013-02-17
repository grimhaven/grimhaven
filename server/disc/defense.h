#ifndef __DISC_DEFENSE_H
#define __DISC_DEFENSE_H

#include "misc/discipline.h"
#include "misc/skills.h"

class CDDefense : public CDiscipline
{
public:
    CSkill skAdvancedDefense;

    CDDefense();
    CDDefense(const CDDefense &a);
    CDDefense & operator=(const CDDefense &a);
    virtual ~CDDefense();
    virtual CDDefense * cloneMe() { return new CDDefense(*this); }

    bool isFast(){ return true; }

private:
};

#endif
