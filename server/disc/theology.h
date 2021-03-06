#ifndef SERVER_DISC_THEOLOGY_H_
#define SERVER_DISC_THEOLOGY_H_

// This is the THEOLOGY discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDTheology : public CDiscipline
{
public:
    CSkill skPenance;
    CSkill skAttune;

    CDTheology()
      : CDiscipline(),
      skPenance(),
      skAttune() {
    }
    CDTheology(const CDTheology &a)
      : CDiscipline(a),
      skPenance(a.skPenance),
      skAttune(a.skAttune) {
    }
    CDTheology & operator=(const CDTheology &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skPenance = a.skPenance;
      skAttune = a.skAttune;
      return *this;
    }
    virtual ~CDTheology() {}
    virtual CDTheology * cloneMe() { return new CDTheology(*this); }

    bool isBasic(){ return true; }

private:
};

void attune(TBeing *, TThing *);

#endif
