#ifndef SERVER_DISC_LORE_H_
#define SERVER_DISC_LORE_H_

// This is the mage/ranger/shaman lore discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDLore : public CDiscipline
{
public:
    CSkill skMeditate;
    CSkill skMana;

    CDLore()
      : CDiscipline(),
      skMeditate(),
      skMana() {
    }
    CDLore(const CDLore &a)
      : CDiscipline(a),
      skMeditate(a.skMeditate),
      skMana(a.skMana) {
    }
    CDLore & operator=(const CDLore &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skMeditate = a.skMeditate;
      skMana = a.skMana;
      return *this;
    }
    virtual ~CDLore() {}
    virtual CDLore * cloneMe() { return new CDLore(*this); }

    bool isBasic(){ return true; }

private:
};

#endif
