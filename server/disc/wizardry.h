#ifndef SERVER_DISC_WIZARDRY_H_
#define SERVER_DISC_WIZARDRY_H_

// This is the WIZARDRY discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDWizardry : public CDiscipline
{
public:
    CSkill skWizardry;

    CDWizardry() :
      CDiscipline(),
      skWizardry()
    {
    }
    CDWizardry(const CDWizardry &a) :
      CDiscipline(a),
      skWizardry(a.skWizardry)
    {
    }
    CDWizardry & operator=(const CDWizardry &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skWizardry = a.skWizardry;
      return *this;
    }
    virtual ~CDWizardry() {}
    virtual CDWizardry * cloneMe() { return new CDWizardry(*this); }

    bool isAutomatic(){ return true; }

private:
};

#endif
