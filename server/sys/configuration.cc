#include <fstream>

#include <boost/program_options.hpp>

#include "sys/configuration.h"

// massage certain options after loading
void Configuration::setCompositeOptions(void) {
    mud_name_version_ = mud_name_ + " v" + mud_version_;

    if (mode_builder_ || mode_beta_)
        mode_prod_ = false;
}

// returns a descriptive message if error, else empty string
sstring Configuration::load(const int argc, const char *argv[]) {
  using std::string;
  namespace po = boost::program_options;

  // command line only options
  po::options_description cmdline_only("Command line only");
  cmdline_only.add_options()
    ("misc/help.h", "produce help message")
    ("config,c", po::value<string>(&config_file_), "configuration file to use")
    ;

  // command line OR in config file
  po::options_description config("Configuration + Command line");
  config.add_options()
    ("lib,l",
      po::value<string>(&data_dir_)->default_value(data_dir_),
      "data directory to run in")
    ("port,p",
      po::value<int>(&game_port_)->default_value(game_port_),
      "telnet protocol game port")
    ("xml_port",
      po::value<int>(&xml_port_)->default_value(xml_port_),
      "xml protocol game port")
    ("prod_mode",
      po::value<bool>(&mode_prod_)->default_value(mode_prod_),
      "run game in production mode")
    ("beta_mode",
      po::value<bool>(&mode_beta_)->default_value(mode_beta_),
      "run game in beta mode")
    ("builder_mode",
      po::value<bool>(&mode_builder_)->default_value(mode_builder_),
      "run game in builder mode")
    ("wizlock,w",
      po::value<bool>(&wizlock_)->default_value(wizlock_),
      "enable wizlock at startup")
    ("wizlock_password",
      po::value<string>(&wizlock_password_)->default_value(wizlock_password_),
      "password to bypass wizlock")
    ("wizlock_message",
      po::value<string>(&wizlock_message_)->default_value(wizlock_message_),
      "reason wizlock is active")
    ("name",
      po::value<string>(&mud_name_)->default_value(mud_name_),
      "name of this MUD instance")
    ("version",
      po::value<string>(&mud_version_)->default_value(mud_version_),
      "version of this MUD instance")
    ("email",
      po::value<string>(&mud_email_)->default_value(mud_email_),
      "email of MUD admins")
    ("url",
      po::value<string>(&mud_url_)->default_value(mud_url_),
      "url of MUD website")
    ;

  // config file only options
  po::options_description config_only("Configuration File Only");
  config_only.add_options()
    ("no_specials,s",
      po::value<bool>(&no_specials_)->default_value(no_specials_),
      "suppress assignment of special routines")
    ("item_damage_rate",
      po::value<int>(&item_damage_rate_)->default_value(item_damage_rate_),
      "see configuration.h")
    ("rent_credit_val",
      po::value<int>(&rent_credit_val_)->default_value(rent_credit_val_),
      "see configuration.h")
    ("rent_sell_to_pawn",
      po::value<bool>(&rent_sell_to_pawn_)->default_value(rent_sell_to_pawn_),
      "see configuration.h")
    ("rent_restrict_inns_by_level",
      po::value<bool>(&rent_restrict_inns_by_level_)
      ->default_value(rent_restrict_inns_by_level_),
      "see configuration.h")
    ("penalize_for_auto_renting",
      po::value<bool>(&penalize_for_auto_renting_)
      ->default_value(penalize_for_auto_renting_),
      "see configuration.h")
    ("weapon_dam_min_hardness",
      po::value<int>(&weapon_dam_min_hardness_)
      ->default_value(weapon_dam_min_hardness_),
      "see configuration.h")
    ("weapon_dam_max_hardness",
      po::value<int>(&weapon_dam_max_hardness_)
      ->default_value(weapon_dam_max_hardness_),
      "see configuration.h")
    ("weapon_dam_max_sharp",
      po::value<int>(&weapon_dam_max_sharp_)
      ->default_value(weapon_dam_max_sharp_),
      "see configuration.h")
    ("nuke_repair_items",
      po::value<bool>(&nuke_repair_items_)->default_value(nuke_repair_items_),
      "see configuration.h")
    ("check_multiplay",
      po::value<bool>(&check_multiplay_)->default_value(check_multiplay_),
      "see configuration.h")
    ("force_multiplay_compliance",
      po::value<bool>(&force_multiplay_compliance_)
        ->default_value(force_multiplay_compliance_),
      "see configuration.h")
    ("repo_mobs",
      po::value<bool>(&repo_mobs_)->default_value(repo_mobs_),
      "see configuration.h")
    ("super_repo_mobs",
      po::value<bool>(&super_repo_mobs_)->default_value(super_repo_mobs_),
      "see configuration.h")
    ("no_damaged_items_shop",
      po::value<bool>(&no_damaged_items_shop_)
      ->default_value(no_damaged_items_shop_),
      "see configuration.h")
    ("auto_deletion",
      po::value<bool>(&auto_deletion_)->default_value(auto_deletion_),
      "see configuration.h")
    ("rent_only_deletion",
      po::value<bool>(&rent_only_deletion_)
      ->default_value(rent_only_deletion_),
      "see configuration.h")
    ("nuke_inactive_mobs",
      po::value<bool>(&nuke_inactive_mobs_)
      ->default_value(nuke_inactive_mobs_),
      "see configuration.h")
    ("load_on_death",
      po::value<bool>(&load_on_death_)->default_value(load_on_death_),
      "see configuration.h")
    ;

  // database options
  po::options_description database("Database");
  database.add_options()
    ("db_host",
      po::value<string>(&db_host_)->default_value(db_host_),
      "host of SQL database server")
    ("db_user",
      po::value<string>(&db_user_)->default_value(db_user_),
      "user name for SQL database connection")
    ("db_name",
      po::value<string>(&db_name_)->default_value(db_name_),
      "name of SQL database")
    ;

  po::options_description cmdline_options;
  cmdline_options.add(cmdline_only).add(config).add(database);

  po::options_description config_options;
  config_options.add(config).add(database).add(config_only);

  po::options_description visible("Available options");
  visible.add(cmdline_only).add(config).add(database).add(config_only);

  po::variables_map vm;

  po::store(po::command_line_parser(argc, argv).
    options(cmdline_options).run(), vm);
  po::notify(vm);

  if (vm.count("help"))
    return format("Usage: %s [options]\n%s") % argv[0] % visible;

  if (vm.count("config")) {
    std::ifstream ifs(config_file_.c_str());

    if (!ifs.is_open())
      return format("Failed to open config file '%s'\n") % config_file_;

    po::store(parse_config_file(ifs, config_options), vm);
    po::notify(vm);
  }

  setCompositeOptions();

  return "";
}
