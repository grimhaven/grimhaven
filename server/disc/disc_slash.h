#ifndef __DISC_SLASH_H
#define __DISC_SLASH_H

// This is the SLASH discipline.

#include "discipline.h"
#include "skills.h"

class CDSlash : public CDiscipline
{
public:
    CSkill skSlashSpec;

    CDSlash();
    CDSlash(const CDSlash &a);
    CDSlash & operator=(const CDSlash &a);
    virtual ~CDSlash();
    virtual CDSlash * cloneMe() { return new CDSlash(*this); }

    bool isFast(){ return true; }

private:
};

#endif
