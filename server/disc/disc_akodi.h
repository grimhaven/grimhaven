#ifndef __DISC_AKODI_H
#define __DISC_AKODI_H

// This is the AKODI discipline.

#include "discipline.h"
#include "skills.h"

class CDAkodi : public CDiscipline
{
public:
    CSkill skMartialSpec;

    CDAkodi()
      : CDiscipline(), skMartialSpec() {
    }
    CDAkodi(const CDAkodi &a)
      : CDiscipline(a), skMartialSpec(a.skMartialSpec) {
    }
    CDAkodi & operator=(const CDAkodi &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skMartialSpec = a.skMartialSpec;
      return *this;
    }
    virtual ~CDAkodi() {}
    virtual CDAkodi * cloneMe() { return new CDAkodi(*this); }

private:
};

#endif

