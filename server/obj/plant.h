#ifndef SERVER_OBJ_PLANT_H_
#define SERVER_OBJ_PLANT_H_

#include "obj/expandable_container.h"


extern int seed_to_plant(int vnum);

class TPlant : public TExpandableContainer {
  private:
  int planttype, plantage, plantyield, verminated;

  public:
    virtual void assignFourValues(int, int, int, int);
    virtual void getFourValues(int *, int *, int *, int *) const;
    virtual sstring statObjInfo() const;
    virtual itemTypeT itemType() const { return ITEM_PLANT; }
    virtual void updateDesc();
    virtual void updateAge();
    virtual int putSomethingInto(TBeing *, TThing *);

    void setType(int t){ planttype=t; }
    int getType() const { return planttype; }
    void setAge(int t){ plantage=t; }
    int getAge() const { return plantage; }
    void setYield(int t){ plantyield=t; }
    int getYield() const { return plantyield; }
    void setVerminated(int t){ verminated=t; }
    int getVerminated() const { return verminated; }
    void peeOnMe(const TBeing *);

    TPlant();
    TPlant(const TPlant &a);
    TPlant & operator=(const TPlant &a);
    virtual ~TPlant();
};



#endif
