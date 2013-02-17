#ifndef SERVER_DISC_COMMONER_H_
#define SERVER_DISC_COMMONER_H_

#include "misc/discipline.h"

class CDCommoner : public CDiscipline
{
public:
    CDCommoner()
      : CDiscipline(){
    }
    CDCommoner(const CDCommoner &a)
      : CDiscipline(a){
    }
    CDCommoner & operator=(const CDCommoner &a) {
      if (this == &a) return *this;
      CDiscipline::operator=(a);
      return *this;
    }
    virtual ~CDCommoner() {}
    virtual CDCommoner * cloneMe() { return new CDCommoner(*this); }

    bool isBasic(){ return true; }

private:
};

#endif
