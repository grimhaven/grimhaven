#ifndef __DISC_JUMANDO_H
#define __DISC_JUMANDO_H

// This is the JUMANDO discipline.

#include "discipline.h"
#include "skills.h"

class CDJumando: public CDiscipline
{
public:
    CSkill skDodge;
    CSkill skShoulderThrow;
    CSkill skFeignDeath;
    CSkill skSpringLeap;
    CSkill skCounterMove;

    CDJumando()
      : CDiscipline(),
      skDodge(),
      skShoulderThrow(),
      skFeignDeath(),
      skSpringLeap(),
      skCounterMove() {
    }
    CDJumando(const CDJumando &a)
      : CDiscipline(a),
      skDodge(a.skDodge),
      skShoulderThrow(a.skShoulderThrow),
      skFeignDeath(a.skFeignDeath),
      skSpringLeap(a.skSpringLeap),
      skCounterMove(a.skCounterMove) {
    }
    CDJumando & operator=(const CDJumando &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skDodge = a.skDodge;
      skShoulderThrow = a.skShoulderThrow;
      skFeignDeath = a.skFeignDeath;
      skSpringLeap = a.skSpringLeap;
      skCounterMove = a.skCounterMove;
      return *this;
    }
    virtual ~CDJumando() {}
    virtual CDJumando * cloneMe() { return new CDJumando(*this); }

private:
};

    int feignDeath(TBeing *);
    int springLeap(TBeing *, TBeing *);
    int shoulderThrow(TBeing *, TBeing *);

#endif

