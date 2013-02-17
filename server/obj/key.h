#ifndef SERVER_OBJ_KEY_H_
#define SERVER_OBJ_KEY_H_

#include "misc/obj.h"


class TKey : public TObj {
  private:
  public:
    virtual void assignFourValues(int, int, int, int);
    virtual void getFourValues(int *, int *, int *, int *) const;
    virtual sstring statObjInfo() const;
    virtual itemTypeT itemType() const { return ITEM_KEY; }
    virtual int putMeInto(TBeing *, TOpenContainer *);

    virtual void lowCheck();
    virtual bool objectRepair(TBeing *, TMonster *, silentTypeT);
    virtual int stealModifier();

    TKey();
    TKey(const TKey &a);
    TKey & operator=(const TKey &a);
    virtual ~TKey();
};



#endif
