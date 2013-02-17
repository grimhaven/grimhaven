#ifndef SERVER_SYS_CONFIGURATION_H_
#define SERVER_SYS_CONFIGURATION_H_

#include "sys/sstring.h"

class Configuration {
 public:
  sstring load(const int argc, const char *argv[]);

  inline bool no_specials() { return no_specials_; }
  inline bool load_on_death() { return load_on_death_; }
  inline bool nuke_inactive_mobs() { return nuke_inactive_mobs_; }
  inline bool rent_only_deletion() { return rent_only_deletion_; }
  inline bool auto_deletion() { return auto_deletion_; }
  inline bool no_damaged_items_shop() { return no_damaged_items_shop_; }
  inline bool repo_mobs() { return repo_mobs_; }
  inline bool super_repo_mobs() { return super_repo_mobs_; }
  inline bool check_multiplay() { return check_multiplay_; }
  inline bool force_multiplay_compliance() { return force_multiplay_compliance_; }
  inline bool nuke_repair_items() { return nuke_repair_items_; }
  inline int item_damage_rate() { return item_damage_rate_; }
  inline int rent_credit_val() { return rent_credit_val_; }
  inline bool rent_sell_to_pawn() { return rent_sell_to_pawn_; }
  inline bool rent_restrict_inns_by_level() { return rent_restrict_inns_by_level_; }
  inline bool penalize_for_auto_renting() { return penalize_for_auto_renting_; }
  inline int weapon_dam_min_hardness() { return weapon_dam_min_hardness_; }
  inline int weapon_dam_max_hardness() { return weapon_dam_max_hardness_; }
  inline int weapon_dam_max_sharpness() { return weapon_dam_max_sharpness_; }
  inline bool mode_builder() { return mode_builder_; }
  inline bool mode_beta() { return mode_beta_; }
  inline bool mode_production() { return mode_production_; }
  inline sstring data_dir() { return data_dir_; }
  inline bool wizlock() { return wizlock_; }
  inline void set_wizlock(bool b) { wizlock_ = b; }
  inline sstring wizlock_password() { return wizlock_password_; }
  inline sstring wizlock_message() { return wizlock_message_; }
  inline void set_wizlock_message(sstring s) { wizlock_message_ = s; }
  inline int game_port() { return game_port_; }
  inline int xml_port() { return xml_port_; }
  inline sstring config_file() { return config_file_; }
  inline sstring db_host() { return db_host_; }
  inline sstring db_user() { return db_user_; }
  inline sstring db_name() { return db_name_; }
  inline sstring mud_name() { return mud_name_; }
  inline sstring mud_version() { return mud_version_; }
  inline sstring mud_name_version() { return mud_name_version_; }
  inline sstring mud_email() { return mud_email_; }

  Configuration() {};
 private:
  void setCompositeOptions();

  // modifies rate at which items take damage
  // The higher the number, the lower the damage
  // Any hit doing less then this amount has no chance of damaging
  // All other hits get modifier on damage rate based on this value
  int item_damage_rate_ = 1;

  // used to determine rent credit
  // credit = level * maxi(20, level) * x
  // the values of the "model" items (soft leather, plate, etc) were set based
  // on this rent credit being at 75
  // 4.0's no rent, and 4.1's big rent credit have distorted this so feel free
  // to adjust if this no longer works
  int rent_credit_val_ = 75;

  // if player goes over rent, items are "sold" to pay for it
  // this handles what is done with the sold item(s)
  // If turned on, the pawnguy gets them
  // otherwise they get deleted
  bool rent_sell_to_pawn_ = false;

  // causes innkeepers to grant rentCredit based on the innkeepers levels
  // otherwise, it is based on the players level.
  // the chief use of this is to encourage high level pc's to use certain inns
  bool rent_restrict_inns_by_level_ = false;

  // causes bad things to happen to player based on time in autorent
  // there is a grace period to handle crashes
  bool penalize_for_auto_renting_ = true;

  // the minimum "hardness" for a material to damage/blunt a weapon
  // when hitting.
  int weapon_dam_min_hardness_ = 20;

  // the max value of a hardness roll, raising it = weapon
  // damage/blunt DECREASE
  int weapon_dam_max_hardness_ = 150;

  // the max value of a sharpness roll, raising it = weapon blunt DECREASE
  int weapon_dam_max_sharpness_ = 150;

  // causes items left in repair to be deleted after a set number of days.
  // Good to keep things circulating, but bad if extended downtime anticipated.
  // Simply deletes the file in mobdata/repairs/, the tickets still exist
  // and the repairman will say he doesn't have the item.
  bool nuke_repair_items_ = true;

  // Enables a check to validate that players are not multiplaying.
  // check is done each login and periodically for all chars logged in.
  bool check_multiplay_ = true;

  // code will disallow any bad multiplay event
  bool force_multiplay_compliance_ = true;

  // Enables automatic generation of repossession mobs based on item max-exist.
  // Not extremely popular, but a good way to get item overturn.
  bool repo_mobs_ = false;

  // items that are over max-exist get hunted by a buffed up version of the
  // hunter.  Requires repo_mobs be TRUE.
  // VERY unpopular
  bool super_repo_mobs_ = false;

  // shops tend to get a lot of goods that strictly speaking aren't isSimilar()
  // slightly damaged, depreciated, etc.
  // We can eliminate this by turning this on.  Any item not perfect will get
  // deleted.
  bool no_damaged_items_shop_ = false;

  // causes player/rent files to be automatically purged if inactive for
  // more then a few weeks.  Conserves disk space and speeds up the boot
  // process significantly.  Periods of college breaks are bypassed.
  bool auto_deletion_ = false;

  // requires auto-deletion turned on causes deletion only of the rent
  // file.  Otherwise pfile, rent and account go
  bool rent_only_deletion_ = false;

  // Causes mobs in inactive zones to be deleted.  Typically, 50% of the mud's
  // mobs would qualify.  Dramatically speeds up the mobileActivity loop and
  // improves CPU performance.
  bool nuke_inactive_mobs_ = false;

  // Causes mobs to drop their zonefile loaded gear and randomly
  // generated loot when they die, and not when they spawn.  the
  // skills spy and steal also trigger off of this setting to make
  // their behavior usful when mobs have no gear.  turning this on
  // makes checking loot loads impossible, makes loot appear more
  // 'random' reduces server memory footprint, reduces load time (from
  // having to calculate loot at spawn), and keeps loot items from
  // scapping during a fight
  bool load_on_death_ = true;

  // suppress assigning of special routines
  bool no_specials_ = false;

  // data directory to run in (eg "lib")
  sstring data_dir_ = "lib";

  // config file that options were loaded from
  sstring config_file_ = "";

  // whether wizlock is enabled
  bool wizlock_ = false;

  // password to bypass wizlock
  sstring wizlock_password_ = "superviii";

  // reason wizlock is active
  sstring wizlock_message_ = "";

  // TCP port which speaks the regular telnet protocol
  int game_port_ = 7900;

  // TCP port which speaks the XML network protocol rather than raw telnet
  int xml_port_ = 0;

  // turn off mail & horsemen & comp placement
  bool mode_builder_ = false;

  // enable various beta-mode more-fun options
  bool mode_beta_ = false;

  // enable various production-mode restrictions
  bool mode_production_ = true;

  // database connection params
  sstring db_host_ = "localhost";
  sstring db_user_ = "grimhaven";
  sstring db_name_ = "grimhaven";

  // mud metadata
  sstring mud_name_ = "Grimhaven";
  sstring mud_version_ = "6.0";
  sstring mud_email_ = "mud@grimhaven.org";
  sstring mud_url_ = "http://grimhaven.org";
  sstring mud_name_version_ = mud_name_ + " v" + mud_version_;

};

#endif
