#ifndef __DISC_PIETY_H
#define __DISC_PIETY_H

// This is the FAITH discipline.

#include "discipline.h"
#include "skills.h"

class CDFaith : public CDiscipline
{
public:
    CSkill skDevotion;

    CDFaith() :
      CDiscipline(),
      skDevotion()
    {
    }
    CDFaith(const CDFaith &a) :
      CDiscipline(a),
      skDevotion(a.skDevotion)
    {
    }
    CDFaith & operator=(const CDFaith &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skDevotion = a.skDevotion;
      return *this;
    }
    virtual ~CDFaith() {}
    virtual CDFaith * cloneMe() { return new CDFaith(*this); }

    bool isAutomatic(){ return true; }
private:
};

#endif
