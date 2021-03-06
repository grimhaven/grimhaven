#ifndef SERVER_OBJ_FRUIT_H_
#define SERVER_OBJ_FRUIT_H_

#include "misc/obj.h"
#include "obj/food.h"

class TFruit : public TFood {
  private:
    int seedVNum;
  public:
    virtual void assignFourValues(int, int, int, int);
    virtual void getFourValues(int *, int *, int *, int *) const;
    virtual sstring statObjInfo() const;
    virtual itemTypeT itemType() const { return ITEM_FRUIT; }


    virtual void eatMe(TBeing *);
    virtual void tasteMe(TBeing *);
    virtual int objectDecay();

    void createSeeds();

    int getSeedVNum() const;
    void setSeedVNum(int r);

    TFruit();
    TFruit(const TFruit &a);
    TFruit & operator=(const TFruit &a);
    virtual ~TFruit();

};
#endif
