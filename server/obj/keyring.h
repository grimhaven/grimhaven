#ifndef SERVER_OBJ_KEYRING_H_
#define SERVER_OBJ_KEYRING_H_

#include "obj/expandable_container.h"

// The actual item
class TKeyring : public TExpandableContainer {
  private:
  public:
    virtual void assignFourValues(int, int, int, int);
    virtual void getFourValues(int *, int *, int *, int *) const;
    virtual sstring statObjInfo() const;
    virtual itemTypeT itemType() const { return ITEM_KEYRING; }
    virtual void putMoneyInto(TBeing *, int);
    virtual bool objectRepair(TBeing *, TMonster *, silentTypeT);

    TKeyring();
    TKeyring(const TKeyring &a);
    TKeyring & operator=(const TKeyring &a);
    virtual ~TKeyring();
};



#endif
