#ifndef __DISC_DEIKHAN_FIGHT_H
#define __DISC_DEIKHAN_FIGHT_H

// This is the DEIKHAN FIGHTING skills discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDDeikhanFight : public CDiscipline
{
  public:
    CSkill skSwitchDeikhan;
    CSkill skRetreatDeikhan;
    CSkill skShoveDeikhan;

    CDDeikhanFight()
      : CDiscipline(),
      skSwitchDeikhan(),
      skRetreatDeikhan(),
      skShoveDeikhan()
    {
    }
    CDDeikhanFight(const CDDeikhanFight &a)
      : CDiscipline(a),
      skSwitchDeikhan(a.skSwitchDeikhan),
      skRetreatDeikhan(a.skRetreatDeikhan),
      skShoveDeikhan(a.skShoveDeikhan)
    {
    }
    CDDeikhanFight & operator=(const CDDeikhanFight &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skSwitchDeikhan = a.skSwitchDeikhan;
      skRetreatDeikhan = a.skRetreatDeikhan;
      skShoveDeikhan = a.skShoveDeikhan;
      return *this;
    }
    virtual ~CDDeikhanFight() {}
    virtual CDDeikhanFight * cloneMe() { return new CDDeikhanFight(*this); }

private:
};

#endif
