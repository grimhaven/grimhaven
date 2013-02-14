#ifndef __DISC_BLUNT_H
#define __DISC_BLUNT_H

// This is the BLUNT discipline.

#include "discipline.h"
#include "skills.h"

class CDBash : public CDiscipline
{
public:
    CSkill skBluntSpec;

    CDBash();
    CDBash(const CDBash &a);
    CDBash & operator=(const CDBash &a);
    virtual ~CDBash();
    virtual CDBash * cloneMe() { return new CDBash(*this); }

    bool isFast(){ return true; }

private:
};

#endif

