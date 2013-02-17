#ifndef SERVER_OBJ_JEWELRY_H_
#define SERVER_OBJ_JEWELRY_H_

#include "obj/base_clothing.h"

class TJewelry : public TBaseClothing {
  private:
  public:
    virtual void assignFourValues(int, int, int, int);
    virtual void getFourValues(int *, int *, int *, int *) const;
    virtual sstring statObjInfo() const;
    virtual itemTypeT itemType() const { return ITEM_JEWELRY; }

    virtual void lowCheck();

    TJewelry();
    TJewelry(const TJewelry &a);
    TJewelry & operator=(const TJewelry &a);
    virtual ~TJewelry();
};


#endif
