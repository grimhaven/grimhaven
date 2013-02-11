#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include "sstring.h"

class Config {
 private:
  Config();

  // modifies rate at which items take damage
  // The higher the number, the lower the damage
  // Any hit doing less then this amount has no chance of damaging
  // All other hits get modifier on damage rate based on this value
  // 4.1 balanced at 2 prior to depreciation
  static int item_damage_rate;

  // used to determine rent credit
  // credit = level * maxi(20, level) * x
  // the values of the "model" items (soft leather, plate, etc) were set based
  // on this rent credit being at 75
  // 4.0's no rent, and 4.1's big rent credit have distorted this so feel free
  // to adjust if this no longer works
  static int rent_credit_val;
  
  // if player goes over rent, items are "sold" to pay for it
  // this handles what is done with the sold item(s)
  // If turned on, the pawnguy gets them
  // otherwise they get deleted
  static bool rent_sell_to_pawn;
  
  // causes innkeepers to grant rentCredit based on the innkeepers levels
  // otherwise, it is based on the players level.
  // the chief use of this is to encourage high level pc's to use certain inns
  static bool rent_restrict_inns_by_level;
  
  // causes bad things to happen to player based on time in autorent
  // there is a grace period to handle crashes
  static bool penalize_for_auto_renting;
  
  // the minimum "hardness" for a material to damage/blunt a weapon
  // when hitting.
  static int weapon_dam_min_hardness;
  
  // the max value of a hardness roll, raising it = weapon
  // damage/blunt DECREASE
  static int weapon_dam_max_hardness;
  
  // the max value of a sharpness roll, raising it = weapon blunt DECREASE
  static int weapon_dam_max_sharp;
  
  // speef is too important for comments so we don't know what this does
  static bool speef_make_body;
  
  // causes items left in repair to be deleted after a set number of days.
  // Good to keep things circulating, but bad if extended downtime anticipated.
  // Simply deletes the file in mobdata/repairs/, the tickets still exist
  // and the repairman will say he doesn't have the item.
  static bool nuke_repair_items;
  
  // Enables a check to validate that players are not multiplaying. 
  // check is done each login and periodically for all chars logged in.
  static bool check_multiplay;
  
  // code will disallow any bad multiplay event
  static bool force_multiplay_compliance;

  // Enables automatic generation of repossession mobs based on item max-exist.
  // Not extremely popular, but a good way to get item overturn.
  static bool repo_mobs;
  
  // items that are over max-exist get hunted by a buffed up version of the
  // hunter.  Requires repo_mobs be TRUE.
  // VERY unpopular
  static bool super_repo_mobs;
  
  // shops tend to get a lot of goods that strictly speaking aren't isSimilar()
  // slightly damaged, depreciated, etc.
  // We can eliminate this by turning this on.  Any item not perfect will get
  // deleted.
  static bool no_damaged_items_shop;

  // causes player/rent files to be automatically purged if inactive for
  // more then a few weeks.  Conserves disk space and speeds up the boot
  // process significantly.  Periods of college breaks are bypassed.
  static bool auto_deletion;
  
  // requires auto-deletion turned on causes deletion only of the rent
  // file.  Otherwise pfile, rent and account go
  static bool rent_only_deletion;
  
  // Causes mobs in inactive zones to be deleted.  Typically, 50% of the mud's
  // mobs would qualify.  Dramatically speeds up the mobileActivity loop and
  // improves CPU performance.
  static bool nuke_inactive_mobs;

  // Causes mobs to drop their zonefile loaded gear and randomly
  // generated loot when they die, and not when they spawn.  the
  // skills spy and steal also trigger off of this setting to make
  // their behavior usful when mobs have no gear.  turning this on
  // makes checking loot loads impossible, makes loot appear more
  // 'random' reduces server memory footprint, reduces load time (from
  // having to calculate loot at spawn), and keeps loot items from
  // scapping during a fight
  static bool load_on_death;

  // whether or not boost::format will throw exceptions for bad format
  // strings, extra or missing arguments etc.  false for maximum
  // stability, true for maximum bug detection.
  static bool throw_format_exceptions;

  // suppress assigning of special routines
  static bool no_specials;

  // data directory to run in (eg "lib")
  static sstring data_dir;

  // config file that options were loaded from
  static sstring config_file;

  // password to bypass wizlock
  static sstring wizlock_password;

  // TCP port which speaks the XML network protocol rather than raw telnet
  static int xml_port;

  // turn off mail & horsemen & comp placement
  static bool mode_builder;

  // enable various beta-mode more fun options
  static bool mode_beta;

  // remove various production-mode restrictions
  static bool mode_prod;

 public:
  static bool doConfiguration(int argc=0, char *argv[]=0);
  
  static int ItemDamageRate(){ return item_damage_rate; }
  static int RentCreditVal(){ return rent_credit_val;}
  static bool RentSellToPawn(){ return rent_sell_to_pawn;}
  static bool RentRestrictInnsByLevel(){return rent_restrict_inns_by_level;}
  static bool PenalizeForAutoRenting(){ return penalize_for_auto_renting;}
  static int WeaponDamMinHardness(){ return weapon_dam_min_hardness;}
  static int WeaponDamMaxHardness(){ return weapon_dam_max_hardness;}
  static int WeaponDamMaxSharp(){ return weapon_dam_max_sharp;}
  static bool SpeefMakeBody(){ return speef_make_body;}
  static bool NukeRepairItems(){ return nuke_repair_items; }
  static bool CheckMultiplay(){ return check_multiplay; }
  static bool ForceMultiplayCompliance(){return force_multiplay_compliance;}
  static bool RepoMobs(){ return repo_mobs; }
  static bool SuperRepoMobs(){ return super_repo_mobs; }
  static bool NoDamagedItemsShop(){ return no_damaged_items_shop; }
  static bool AutoDeletion(){ return auto_deletion; }
  static bool RentOnlyDeletion(){ return rent_only_deletion; }
  static bool NukeInactiveMobs(){ return nuke_inactive_mobs; }
  static bool LoadOnDeath(){ return load_on_death; }
  static bool ThrowFormatExceptions(){ return throw_format_exceptions; }
  static bool NoSpecials(){ return no_specials; }
  static sstring DataDir(){ return data_dir; }
  static sstring ConfigFile(){ return config_file; }
  static sstring WizLockPassword(){ return wizlock_password; }
  static bool XmlPort(){ return xml_port; }
  static bool ModeBuilder(){ return mode_builder; }
  static bool ModeBeta(){ return mode_beta; }
  // builder & beta mode exclude prod mode
  static bool ModeProd(){ return !(mode_builder || mode_beta) && mode_prod; }
};




#endif
