#ifndef SERVER_MISC_SYS_LOOT_H_
#define SERVER_MISC_SYS_LOOT_H_

struct TLootStructure {
  int tLevel, // Object Level;
      tRNum;  // Object RealNumber;

  TLootStructure * tNext;
};

extern TLootStructure * tLoot;

extern bool sysLootBoot();
extern bool sysLootLoad(resetCom &, TBeing *, TObj *, bool);
extern bool isLegalLoot(itemTypeT);
extern bool isLegalLoot(int);

#endif
