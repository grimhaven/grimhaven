#ifndef SERVER_DISC_DEIKHAN_WRATH_H_
#define SERVER_DISC_DEIKHAN_WRATH_H_

// This is the DEIKHAN WRATH discipline.

#include "misc/discipline.h"
#include "misc/skills.h"

class CDDeikhanWrath : public CDiscipline
{
public:
    CSkill skHarmDeikhan;
    CSkill skNumbDeikhan;
    CSkill skEarthquakeDeikhan;
    CSkill skCallLightningDeikhan;
    CSkill skHolyLight;    // not coded

    CDDeikhanWrath()
      : CDiscipline(),
      skHarmDeikhan(),
      skNumbDeikhan(),
      skEarthquakeDeikhan(),
      skCallLightningDeikhan(),
      skHolyLight() {
    }
    CDDeikhanWrath(const CDDeikhanWrath &a)
      : CDiscipline(a),
      skHarmDeikhan(a.skHarmDeikhan),
      skNumbDeikhan(a.skNumbDeikhan),
      skEarthquakeDeikhan(a.skEarthquakeDeikhan),
      skCallLightningDeikhan(a.skCallLightningDeikhan),
      skHolyLight(a.skHolyLight) {
    }
    CDDeikhanWrath & operator=(const CDDeikhanWrath &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skHarmDeikhan = a.skHarmDeikhan;
      skNumbDeikhan = a.skNumbDeikhan;
      skEarthquakeDeikhan = a.skEarthquakeDeikhan;
      skCallLightningDeikhan = a.skCallLightningDeikhan;
      skHolyLight = a.skHolyLight;
      return *this;
    }
    virtual ~CDDeikhanWrath() {}
    virtual CDDeikhanWrath * cloneMe() { return new CDDeikhanWrath(*this); }

private:
};

#endif
