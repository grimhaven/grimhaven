#ifndef SERVER_OBJ_WAGON_H_
#define SERVER_OBJ_WAGON_H_

#include "obj/open_container.h"

class TWagon : public TOpenContainer {
  private:
  public:
    virtual void assignFourValues(int, int, int, int);
    virtual void getFourValues(int *, int *, int *, int *) const;
    virtual sstring statObjInfo() const;
    virtual itemTypeT itemType() const { return ITEM_WAGON; }

    virtual bool objectRepair(TBeing *, TMonster *, silentTypeT);

    TWagon();
    TWagon(const TWagon &a);
    TWagon & operator=(const TWagon &a);
    virtual ~TWagon();
};


#endif
