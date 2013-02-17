#ifndef SERVER_DISC_MURDER_H_
#define SERVER_DISC_MURDER_H_

// This is the MURDER discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDMurder : public CDiscipline
{
public:
    CSkill skGarrotte;
    CSkill skThroatSlit;

    CDMurder()
      : CDiscipline(),
      skGarrotte(),
      skThroatSlit() {
    }
    CDMurder(const CDMurder &a)
      : CDiscipline(a),
      skGarrotte(a.skGarrotte),
      skThroatSlit(a.skThroatSlit) {
    }
    CDMurder & operator=(const CDMurder &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skGarrotte = a.skGarrotte;
      skThroatSlit = a.skThroatSlit;
      return *this;
    }
    virtual ~CDMurder() {}
    virtual CDMurder * cloneMe() { return new CDMurder(*this); }
private:
};

int backstab(TBeing *, TBeing *);
int throatSlit(TBeing *, TBeing *);
int poisonWeapon(TBeing *, TThing *, TThing *);
int garrotte(TBeing *, TBeing *);
int cudgel(TBeing *, TBeing *);
bool addPoison(affectedData aff[5], liqTypeT liq, int level, int duration);

#endif
