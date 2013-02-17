#ifndef SERVER_OBJ_HANDGONNE_H_
#define SERVER_OBJ_HANDGONNE_H_

#include "obj/gun.h"


class THandgonne : public TGun {
 public:
  virtual void loadMe(TBeing *ch, TAmmo *ammo);
  virtual void unloadMe(TBeing *ch, TAmmo *ammo);

  virtual itemTypeT itemType() const { return ITEM_HANDGONNE; }

  int shootMeBow(TBeing *, TBeing *, unsigned int, dirTypeT, int);

  THandgonne();
  THandgonne(const THandgonne &a);
  THandgonne & operator=(const THandgonne &a);
  virtual ~THandgonne();
};


#endif
