#ifndef SERVER_DISC_HAND_OF_GOD_H_
#define SERVER_DISC_HAND_OF_GOD_H_

// This is the HAND_OF_GOD discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDHand : public CDiscipline
{
public:
    CSkill skHeroesFeast;
    CSkill skAstralWalk;
    CSkill skPortal;

    CDHand()
      : CDiscipline(),
      skHeroesFeast(),
      skAstralWalk(),
      skPortal() {
    }
    CDHand(const CDHand &a)
      : CDiscipline(a),
      skHeroesFeast(a.skHeroesFeast),
      skAstralWalk(a.skAstralWalk),
      skPortal(a.skPortal) {
    }
    CDHand & operator=(const CDHand &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skHeroesFeast = a.skHeroesFeast;
      skAstralWalk = a.skAstralWalk;
      skPortal = a.skPortal;
      return *this;
    }
    virtual ~CDHand() {}
    virtual CDHand * cloneMe() { return new CDHand(*this); }
private:
};

int astralWalk(TBeing *, TBeing *);
int astralWalk(TBeing *, TBeing *, int, short);

void createFood(TBeing *);
int createFood(TBeing *, int, short, spellNumT);

void createWater(TBeing *, TObj *);
int castCreateWater(TBeing *, TObj *);
int createWater(TBeing *, TObj *, int, short, spellNumT);

void wordOfRecall(TBeing *, TBeing *);
void wordOfRecall(TBeing *, TBeing *, TMagicItem *);
int wordOfRecall(TBeing *, TBeing *, int, short);

int summon(TBeing *, TBeing *);
int summon(TBeing *, TBeing *, int, short);

void heroesFeast(TBeing *);
int heroesFeast(TBeing *, int, short, spellNumT);

void portal(TBeing *, const char *);
int portal(TBeing *, const char *, int, short);

#endif
