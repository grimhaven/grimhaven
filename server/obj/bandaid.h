#ifndef SERVER_OBJ_BANDAID_H_
#define SERVER_OBJ_BANDAID_H_

#include "misc/obj.h"

class TBandaid : public TObj {
  public:
    virtual void assignFourValues(int, int, int, int);
    virtual void getFourValues(int *, int *, int *, int *) const;
    virtual sstring statObjInfo() const;
    virtual itemTypeT itemType() const { return ITEM_BANDAGE; }

    virtual int removeMe(TBeing *, wearSlotT);
    virtual void scrapMe(TBeing *);
    virtual void findBandage(int *);
    virtual void destroyBandage(int *);

    TBandaid();
    TBandaid(const TBandaid &a);
    TBandaid & operator=(const TBandaid &a);
    virtual ~TBandaid();
};



#endif
