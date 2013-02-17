#ifndef __DISC_PIERCE_H
#define __DISC_PIERCE_H

// This is the PIERCE discipline.

#include "discipline.h"
#include "skills.h"

class CDPierce : public CDiscipline
{
public:
    CSkill skPierceSpec;

    CDPierce();
    CDPierce(const CDPierce &a);
    CDPierce & operator=(const CDPierce &a);
    virtual ~CDPierce();
    virtual CDPierce * cloneMe() { return new CDPierce(*this); }

    bool isFast(){ return true; }

private:
};

#endif
