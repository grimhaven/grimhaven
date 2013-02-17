#ifndef SERVER_OBJ_CANNON_H_
#define SERVER_OBJ_CANNON_H_

#include "obj/gun.h"


class TCannon : public TGun {
 public:
  virtual void loadMe(TBeing *ch, TAmmo *ammo);
  virtual void unloadMe(TBeing *ch, TAmmo *ammo);

  virtual itemTypeT itemType() const { return ITEM_CANNON; }

  int shootMeBow(TBeing *, TBeing *, unsigned int, dirTypeT, int);

  TCannon();
  TCannon(const TCannon &a);
  TCannon & operator=(const TCannon &a);
  virtual ~TCannon();
};


#endif
