#ifndef SERVER_DISC_AEGIS_H_
#define SERVER_DISC_AEGIS_H_

// This is the AEGIS discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDAegis : public CDiscipline
{
public:
    CSkill skSanctuary;
    CSkill skCureParalyze;
    CSkill skSecondWind;
    CSkill skRelive;

    CDAegis()
      : CDiscipline(),
        skSanctuary(), skCureParalyze(),
        skSecondWind(), skRelive() {
    }
    CDAegis(const CDAegis &a)
      : CDiscipline(a),
        skSanctuary(a.skSanctuary), skCureParalyze(a.skCureParalyze),
        skSecondWind(a.skSecondWind), skRelive(a.skRelive) {
    }
    CDAegis & operator=(const CDAegis &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a),
      skSanctuary = a.skSanctuary;
      skCureParalyze = a.skCureParalyze;
      skSecondWind = a.skSecondWind;
      skRelive = a.skRelive;
      return *this;
    }
    virtual ~CDAegis() {}
    virtual CDAegis * cloneMe() { return new CDAegis(*this); }
private:
};

    void armor(TBeing *, TBeing *);
    void armor(TBeing *, TBeing *, TMagicItem * obj, spellNumT);
    int armor(TBeing *, TBeing *, int, short, spellNumT);

    void sanctuary(TBeing *, TBeing *);
    void sanctuary(TBeing *, TBeing *, TMagicItem * obj);
    int sanctuary(TBeing *, TBeing *, int, short);

    void bless(TBeing *, TObj *);
    void bless(TBeing *, TObj *, TMagicItem *, spellNumT);
    int bless(TBeing *, TObj *, int, short, spellNumT);

    void bless(TBeing *, TBeing *);
    void bless(TBeing *, TBeing *, TMagicItem * obj, spellNumT);
    int bless(TBeing *, TBeing *, int, short, spellNumT);

    void cureBlindness(TBeing *, TBeing *);
    void cureBlindness(TBeing *, TBeing *, TMagicItem *);
    int cureBlindness(TBeing *, TBeing *, int, short);

    void curePoison(TBeing *, TBeing *);
    void curePoison(TBeing *, TBeing *, TMagicItem *, spellNumT);
    int curePoison(TBeing *, TBeing *, int, short, spellNumT);

    void refresh(TBeing *, TBeing *);
    void refresh(TBeing *, TBeing *, TMagicItem *, spellNumT);
    int refresh(TBeing *, TBeing *, int, short, spellNumT);

    void secondWind(TBeing *, TBeing *);
    void secondWind(TBeing *, TBeing *, TMagicItem *);
    int secondWind(TBeing *, TBeing *, int, short);

    void cureParalysis(TBeing *, TBeing *);
    void cureParalysis(TBeing *, TBeing *, TMagicItem *);
    int cureParalysis(TBeing *, TBeing *, int, short);

    void cureDisease(TBeing *, TBeing *);
    int  cureDisease(TBeing *, TBeing *, int, short, spellNumT);
    void cureDisease(TBeing *, TBeing *, TMagicItem *, spellNumT);

    void relive(TBeing *, TBeing *);
#endif
