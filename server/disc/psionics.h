#ifndef SERVER_DISC_PSIONICS_H_
#define SERVER_DISC_PSIONICS_H_

#include "misc/discipline.h"
#include "misc/skills.h"

class CDPsionics : public CDiscipline
{
public:
  CSkill skTelepathy;
  CSkill skTeleSight;
  CSkill skTeleVision;
  CSkill skMindFocus;
  CSkill skPsiBlast;
  CSkill skMindThrust;
  CSkill skPsyCrush;
  CSkill skKineticWave;
  CSkill skMindPreservation;
  CSkill skTelekinesis;
  CSkill skPsiDrain;

    CDPsionics();
    CDPsionics(const CDPsionics &a);
    CDPsionics & operator=(const CDPsionics &a);
    virtual ~CDPsionics();
    virtual CDPsionics * cloneMe() { return new CDPsionics(*this); }

    bool isFast(){ return true; }

private:
};

#endif
