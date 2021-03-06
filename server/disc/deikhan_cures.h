#ifndef SERVER_DISC_DEIKHAN_CURES_H_
#define SERVER_DISC_DEIKHAN_CURES_H_

// This is the DEIKHAN CURES discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDDeikhanCures : public CDiscipline
{
public:
    CSkill skSalveDeikhan;
    CSkill skLayHands;

    CDDeikhanCures()
      : CDiscipline(),
      skSalveDeikhan(),
      skLayHands() {
    }
    CDDeikhanCures(const CDDeikhanCures &a)
      : CDiscipline(a),
      skSalveDeikhan(a.skSalveDeikhan),
      skLayHands(a.skLayHands) {
    }
    CDDeikhanCures & operator=(const CDDeikhanCures &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skSalveDeikhan = a.skSalveDeikhan;
      skLayHands = a.skLayHands;
      return *this;
    }
    virtual ~CDDeikhanCures() {}
    virtual CDDeikhanCures * cloneMe() { return new CDDeikhanCures(*this); }

private:
};

#endif
