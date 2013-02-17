#ifndef SERVER_DISC_STEALTH_H_
#define SERVER_DISC_STEALTH_H_

// This is the THIEF Stealth discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

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
