#ifndef SERVER_DISC_PIERCE_H_
#define SERVER_DISC_PIERCE_H_

// This is the PIERCE discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

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
