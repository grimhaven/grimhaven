#ifndef __DISC_STEALTH_H
#define __DISC_STEALTH_H

// This is the THIEF Stealth discipline.

#include "discipline.h"
#include "skills.h"

class CDStealth : public CDiscipline
{
public:
    CSkill skConcealment;
    CSkill skDisguise;

    CDStealth()
      : CDiscipline(),
      skConcealment(),
      skDisguise() {
    }
    CDStealth(const CDStealth &a)
      : CDiscipline(a),
      skConcealment(a.skConcealment),
      skDisguise(a.skDisguise) {
    }
    CDStealth & operator=(const CDStealth &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skConcealment = a.skConcealment;
      skDisguise = a.skDisguise;
      return *this;
    }
    virtual ~CDStealth() {}
    virtual CDStealth * cloneMe() { return new CDStealth(*this); }
private:
};


int conceal(TBeing *, TBeing *);


#endif
