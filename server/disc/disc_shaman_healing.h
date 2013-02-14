#ifndef __DISC_SHAMAN_HEALING_H
#define __DISC_SHAMAN_HEALING_H

// This is the SHAMAN CURES discipline.

#include "discipline.h"
#include "skills.h"

class CDShamanHealing : public CDiscipline
{
public:

    CSkill skHealingGrasp;
    CSkill skEnliven;

    CDShamanHealing()
      : CDiscipline(),
      skHealingGrasp(),
      skEnliven() {
    }
    CDShamanHealing(const CDShamanHealing &a)
      : CDiscipline(a),
      skHealingGrasp(a.skHealingGrasp),
      skEnliven(a.skEnliven) {
    }
    CDShamanHealing & operator=(const CDShamanHealing &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skHealingGrasp = a.skHealingGrasp;
      skEnliven = a.skEnliven;
      return *this;
    }
    virtual ~CDShamanHealing() {}
    virtual CDShamanHealing * cloneMe() { return new CDShamanHealing(*this); }

private:
};
   void healingGrasp(TBeing *, TBeing *);
   int castHealingGrasp(TBeing *, TBeing *);
   void healingGrasp(TBeing *, TBeing *, TMagicItem *, spellNumT);
   int healingGrasp(TBeing *, TBeing *, int, short, spellNumT, int=0);

   int enliven(TBeing *, TBeing *, int, short);
   void enliven(TBeing *, TBeing *, TMagicItem *);
   int enliven(TBeing *, TBeing *);
   int castEnliven(TBeing *, TBeing *);
#endif
