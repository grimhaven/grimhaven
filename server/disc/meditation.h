#ifndef SERVER_DISC_MEDITATION_H_
#define SERVER_DISC_MEDITATION_H_

#include "misc/discipline.h"
#include "misc/skills.h"

class CDMeditationMonk: public CDiscipline
{
public:
    CSkill skWohlin;          // Advanced Yoginsa (meditation)
    CSkill skVoplat;          // Advanced Kubo (makes kubo damage magical)
    CSkill skBlindfighting;

    CDMeditationMonk() :
      CDiscipline(),
      skWohlin(),
      skVoplat(),
      skBlindfighting()
    {
    }
    CDMeditationMonk(const CDMeditationMonk & a) :
      CDiscipline(a),
      skWohlin(a.skWohlin),
      skVoplat(a.skVoplat),
      skBlindfighting(a.skBlindfighting)
    {
    }
    CDMeditationMonk & operator=(const CDMeditationMonk & a)
    {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      skWohlin = a.skWohlin;
      skVoplat = a.skVoplat;
      skBlindfighting = a.skBlindfighting;
      return *this;
    }
    virtual ~CDMeditationMonk() {}
    virtual CDMeditationMonk * cloneMe() { return new CDMeditationMonk(*this); }

private:
};

#endif
