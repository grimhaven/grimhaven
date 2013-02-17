#ifndef __DISC_BLACKSMITHING_H
#define __DISC_BLACKSMITHING_H

// This is the blacksmith discipline.

#include "discipline.h"
#include "skills.h"


class CDBlacksmithing : public CDiscipline
{
public:
  CSkill skBlacksmithingAdvanced;
    CDBlacksmithing();
    CDBlacksmithing(const CDBlacksmithing &a);
    CDBlacksmithing & operator=(const CDBlacksmithing &a);
    virtual ~CDBlacksmithing();
    virtual CDBlacksmithing * cloneMe() { return new CDBlacksmithing(*this); }

private:
};

#endif
