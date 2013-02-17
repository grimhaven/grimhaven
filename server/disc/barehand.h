#ifndef __DISC_BAREHAND_H
#define __DISC_BAREHAND_H

// This is the BAREHAND discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDBarehand : public CDiscipline
{
public:
    CSkill skBarehandSpec;

    CDBarehand()
      : CDiscipline(),
      skBarehandSpec() {
    }
    CDBarehand(const CDBarehand &a)
      : CDiscipline(a),
      skBarehandSpec(a.skBarehandSpec) {
    }
    CDBarehand & operator=(const CDBarehand &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skBarehandSpec = a.skBarehandSpec;
      return *this;
    }
    virtual ~CDBarehand() {}
    virtual CDBarehand * cloneMe() { return new CDBarehand(*this); }

    bool isFast(){ return true; }

private:
};

#endif
