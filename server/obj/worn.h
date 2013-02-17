#ifndef SERVER_OBJ_WORN_H_
#define SERVER_OBJ_WORN_H_

#include "obj/base_clothing.h"


class TWorn : public TBaseClothing {
  private:
  public:
    virtual void assignFourValues(int, int, int, int);
    virtual void getFourValues(int *, int *, int *, int *) const;
    virtual sstring statObjInfo() const;
    virtual itemTypeT itemType() const { return ITEM_WORN; }

    virtual void lowCheck();

    TWorn();
    TWorn(const TWorn &a);
    TWorn & operator=(const TWorn &a);
    virtual ~TWorn();
};


#endif
