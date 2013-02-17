#ifndef SERVER_DISC_BLACKSMITHING_H_
#define SERVER_DISC_BLACKSMITHING_H_

// This is the blacksmith discipline.

#include "misc/discipline.h"
#include "misc/skills.h"


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
