#ifndef __DISC_DEIKHAN_AEGIS_H
#define __DISC_DEIKHAN_AEGIS_H

// This is the DEIKHAN AEGIS discipline.

#include "discipline.h"
#include "skills.h"

class CDDeikhanAegis : public CDiscipline
{
public:
    CSkill skHeroesFeastDeikhan;
    CSkill skRefreshDeikhan;
    CSkill skSynostodweomer;

    CDDeikhanAegis()
      : CDiscipline(),
      skHeroesFeastDeikhan(),
      skRefreshDeikhan(),
      skSynostodweomer() {
    }
    CDDeikhanAegis(const CDDeikhanAegis &a)
      : CDiscipline(a),
      skHeroesFeastDeikhan(a.skHeroesFeastDeikhan),
      skRefreshDeikhan(a.skRefreshDeikhan),
      skSynostodweomer(a.skSynostodweomer) {
    }
    CDDeikhanAegis & operator=(const CDDeikhanAegis &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skHeroesFeastDeikhan = a.skHeroesFeastDeikhan;
      skRefreshDeikhan = a.skRefreshDeikhan;
      skSynostodweomer = a.skSynostodweomer;
      return *this;
    }
    virtual ~CDDeikhanAegis() {}
    virtual CDDeikhanAegis * cloneMe() { return new CDDeikhanAegis(*this); }

private:
};

#endif
