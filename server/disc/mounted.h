#ifndef __DISC_MOUNTED_H
#define __DISC_MOUNTED_H

// This is the DEIKHAN MOUNT discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDMounted : public CDiscipline
{
public:
    CSkill skCalmMount;
    CSkill skTrainMount;
    CSkill skAdvancedRiding;
    CSkill skRideDomestic;
    CSkill skRideNonDomestic;
    CSkill skRideWinged;
    CSkill skRideExotic;

    CDMounted()
      : CDiscipline(),
      skCalmMount(),
      skTrainMount(),
      skAdvancedRiding(),
      skRideDomestic(),
      skRideNonDomestic(),
      skRideWinged(),
      skRideExotic() {
    }
    CDMounted(const CDMounted &a)
      : CDiscipline(a),
      skCalmMount(a.skCalmMount),
      skTrainMount(a.skTrainMount),
      skAdvancedRiding(a.skAdvancedRiding),
      skRideDomestic(a.skRideDomestic),
      skRideNonDomestic(a.skRideNonDomestic),
      skRideWinged(a.skRideWinged),
      skRideExotic(a.skRideExotic) {
    }
    CDMounted & operator=(const CDMounted &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skCalmMount = a.skCalmMount;
      skTrainMount = a.skTrainMount;
      skAdvancedRiding = a.skAdvancedRiding;
      skRideDomestic = a.skRideDomestic;
      skRideNonDomestic = a.skRideNonDomestic;
      skRideWinged = a.skRideWinged;
      skRideExotic = a.skRideExotic;
      return *this;
    }
    virtual ~CDMounted() {}
    virtual CDMounted * cloneMe() { return new CDMounted(*this); }

private:
};

#endif
