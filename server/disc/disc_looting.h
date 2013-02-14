#ifndef __DISC_LOOTING_H
#define __DISC_LOOTING_H

// This is the LOOTING discipline.

#include "discipline.h"
#include "skills.h"

class CDLooting : public CDiscipline
{
public:
  CSkill skCounterSteal;
  CSkill skPlant;

    CDLooting()
      : CDiscipline(),
      skCounterSteal(),
      skPlant(){
    }
    CDLooting(const CDLooting &a)
      : CDiscipline(a),
      skCounterSteal(a.skCounterSteal),
      skPlant(a.skPlant){
    }
    CDLooting & operator=(const CDLooting &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skCounterSteal = a.skCounterSteal;
      skPlant = a.skPlant;
      return *this;
    }
    virtual ~CDLooting() {}
    virtual CDLooting * cloneMe() { return new CDLooting(*this); }

private:
};

    int detectSecret(TBeing *);

    int disarmTrapObj(TBeing *, TObj *);
    int disarmTrapDoor(TBeing *, dirTypeT);

    int detectTrapObj(TBeing *, const TThing *);
    int detectTrapDoor(TBeing *, int);

#endif

