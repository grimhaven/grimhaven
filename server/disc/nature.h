#ifndef SERVER_DISC_NATURE_H_
#define SERVER_DISC_NATURE_H_

// This is the NATURE discipline.
// Most of these were moved to armadillo for shaman
// reserving disc nature for ranger use

#include "misc/discipline.h"
#include "misc/skills.h"

class CDNature : public CDiscipline
{
public:
    CSkill skTreeWalk;



    CDNature()
      : CDiscipline(),
      skTreeWalk() {
    }
    CDNature(const CDNature &a)
      : CDiscipline(a),
      skTreeWalk(a.skTreeWalk) {
    }
    CDNature & operator=(const CDNature &a)  {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skTreeWalk = a.skTreeWalk;
      return *this;
    }
    virtual ~CDNature() {}
    virtual CDNature * cloneMe() { return new CDNature(*this); }
private:
};

int treeWalk(TBeing *, const char *, int, short);
int treeWalk(TBeing *, const char *);

int barkskin(TBeing *, TBeing *);
int castBarkskin(TBeing *, TBeing *);
int barkskin(TBeing *, TBeing *, TMagicItem *);
int barkskin(TBeing *, TBeing *, int, short);

#endif
