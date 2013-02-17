#ifndef __DISC_MINDBODY_H
#define __DISC_MINDBODY_H

#include "misc/discipline.h"
#include "misc/skills.h"

class CDMindBody : public CDiscipline
{
public:
  CSkill skFeignDeath;
  CSkill skBlur;

    CDMindBody();
    CDMindBody(const CDMindBody &a);
    CDMindBody & operator=(const CDMindBody &a);
    virtual ~CDMindBody();
    virtual CDMindBody * cloneMe();

private:
};

#endif
