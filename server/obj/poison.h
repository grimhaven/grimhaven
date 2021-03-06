#ifndef SERVER_OBJ_POISON_H_
#define SERVER_OBJ_POISON_H_

#include "obj/base_cup.h"


class TPoison : public TBaseCup {
  public:
    virtual itemTypeT itemType() const { return ITEM_POISON; }
    virtual int shopPrice(int, int, float, const TBeing *) const;
    virtual int sellPrice(int, int, float, const TBeing *);
    virtual int objectSell(TBeing *, TMonster *);


    TPoison();
    TPoison(const TPoison &a);
    TPoison & operator=(const TPoison &a);
    virtual ~TPoison();
};


#endif
