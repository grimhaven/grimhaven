#ifndef SERVER_DISC_MINDBODY_H_
#define SERVER_DISC_MINDBODY_H_

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
