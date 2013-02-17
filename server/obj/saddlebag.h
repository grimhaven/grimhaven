#ifndef SERVER_OBJ_SADDLEBAG_H_
#define SERVER_OBJ_SADDLEBAG_H_

#include "obj/expandable_container.h"

// The actual item
class TSaddlebag : public TExpandableContainer {
  private:
  public:
    virtual void assignFourValues(int, int, int, int);
    virtual void getFourValues(int *, int *, int *, int *) const;
    virtual sstring statObjInfo() const;
    virtual itemTypeT itemType() const { return ITEM_SADDLEBAG; }

    virtual int getMe(TBeing *, TThing *);
    virtual bool objectRepair(TBeing *, TMonster *, silentTypeT);

    TSaddlebag();
    TSaddlebag(const TSaddlebag &a);
    TSaddlebag & operator=(const TSaddlebag &a);
    virtual ~TSaddlebag();
};

#endif