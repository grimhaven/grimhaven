#ifndef __DISC_SHAMAN_ALCHEMY_H
#define __DISC_SHAMAN_ALCHEMY_H

// This is the SHAMAN ALCHEMY discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDShamanAlchemy : public CDiscipline
{
public:
    CSkill skBrew;

    CDShamanAlchemy()
      : CDiscipline(),
      skBrew() {
    }
    CDShamanAlchemy(const CDShamanAlchemy &a)
      : CDiscipline(a),
      skBrew(a.skBrew) {
    }
    CDShamanAlchemy & operator=(const CDShamanAlchemy &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a),
      skBrew = a.skBrew;
      return *this;
    }
    virtual ~CDShamanAlchemy() {}
    virtual CDShamanAlchemy * cloneMe() { return new CDShamanAlchemy(*this); }
private:
};


#endif
