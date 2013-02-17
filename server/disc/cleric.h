#ifndef SERVER_DISC_CLERIC_H_
#define SERVER_DISC_CLERIC_H_

// This is the CLERIC BASIC discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDCleric : public CDiscipline
{
public:
//Level 1
    CSkill skHealLight;       // MOD   1st level individual
    CSkill skHarmLight;       // MOD   1st level individual

//Level 4
    CSkill skCreateFood;
    CSkill skCreateWater;

//Level 5
   CSkill skArmor;

//Level 6
    CSkill skBless;

//Level 8
    CSkill skClot;
    CSkill skRainBrimstone;

//Level 9
    CSkill skHealSerious;    // MOD  10th level individual
    CSkill skHarmSerious;    // MOD  10th level individual

//Level 10
    CSkill skSterilize;
    CSkill skExpel;
    CSkill skCureDisease;

//Level 11

//Level 12
    CSkill skCurse;
    CSkill skRemoveCurse;
    CSkill skCurePoison;


//Level 15
    CSkill skHealCritical;   // MOD  20th level individual
    CSkill skSalve;
    CSkill skPoison;

//Level18
    CSkill skHarmCritical;   // MOD  20th level individual
    CSkill skInfect;
    CSkill skRefresh;

//Level 20
    CSkill skNumb;
    CSkill skDisease;
    CSkill skFlamestrike;
    CSkill skPlagueOfLocusts;

// Level 21
    CSkill skCureBlindness;

//Level 22
    CSkill skSummon;

//Level 25
    CSkill skHeal;           // MOD  30th level individual
    CSkill skParalyzeLimb;
    CSkill skWordOfRecall;

//Level 28
    CSkill skHarm;           // MOD  30th level individual

//Level 30
    CSkill skKnitBone;
    CSkill skBlindness;

    CSkill skRepairCleric;

    CDCleric();
    CDCleric(const CDCleric &a);
    CDCleric & operator=(const CDCleric &a);
    virtual ~CDCleric();
    virtual CDCleric * cloneMe();

    bool isBasic(){ return true; }

private:
};


#endif
