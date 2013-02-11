#include <iostream>
#include <fstream>

#include "configuration.h"
#include "extern.h"
#include "database.h"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

// static data member defs
int Config::item_damage_rate;
int Config::rent_credit_val;
bool Config::rent_sell_to_pawn;
bool Config::rent_restrict_inns_by_level;
bool Config::penalize_for_auto_renting;
int Config::weapon_dam_min_hardness;
int Config::weapon_dam_max_hardness;
int Config::weapon_dam_max_sharp;
bool Config::speef_make_body;
bool Config::nuke_repair_items;
bool Config::check_multiplay;
bool Config::force_multiplay_compliance;
bool Config::repo_mobs;
bool Config::super_repo_mobs;
bool Config::no_damaged_items_shop;
bool Config::auto_deletion;
bool Config::rent_only_deletion;
bool Config::nuke_inactive_mobs;
bool Config::load_on_death;
bool Config::throw_format_exceptions;
bool Config::no_specials;
sstring Config::data_dir;
sstring Config::config_file;
bool Config::no_mail;
bool Config::wizlock;
sstring Config::wizlock_password;

const int Config::Port::PROD=7900;
const int Config::Port::PROD_XML=7901;
const int Config::Port::BETA=5678;
const int Config::Port::BUILDER=8900;

bool Config::doConfiguration(int argc, char *argv[])
{
  using std::string;

  // command line only options
  po::options_description cmdline_only("Command line only");
  cmdline_only.add_options()
    ("help,h", "produce help message")
    ("config,c", po::value<string>(&config_file),
     "configuration file to use")
    ;

  // command line OR in config file
  po::options_description config("Configuration + Command line");
  config.add_options()
    ("lib,l", po::value<string>(&data_dir)->default_value(Path::DATA), 
     "data directory to run in")
    ("no_specials,s", po::value<bool>(&no_specials)->default_value(false),
     "suppress assignment of special routines")
    ("port,p", po::value<int>(&gamePort)->default_value(Config::Port::PROD),
     "game port")
    ("wizlock,w", po::value<bool>(&wizlock)->default_value(false),
     "wizlock game")
    ("wizlock_password", po::value<string>(&wizlock_password)->default_value("superviii"),
     "password to bypass wizlock")
    ;

  // config file only options
  po::options_description config_only("Configuration File Only");
  config_only.add_options()
    ("item_damage_rate", 
     po::value<int>(&item_damage_rate)->default_value(1),
     "see configuration.h")
    ("rent_credit_val",
     po::value<int>(&rent_credit_val)->default_value(75),
     "see configuration.h")
    ("rent_sell_to_pawn",
     po::value<bool>(&rent_sell_to_pawn)->default_value(false),
     "see configuration.h")
    ("rent_restrict_inns_by_level",
     po::value<bool>(&rent_restrict_inns_by_level)->default_value(false),
     "see configuration.h")
    ("penalize_for_auto_renting",
     po::value<bool>(&penalize_for_auto_renting)->default_value(true),
     "see configuration.h")
    ("weapon_dam_min_hardness",
     po::value<int>(&weapon_dam_min_hardness)->default_value(20),
     "see configuration.h")
    ("weapon_dam_max_hardness",
     po::value<int>(&weapon_dam_max_hardness)->default_value(150),
     "see configuration.h")
    ("weapon_dam_max_sharp",
     po::value<int>(&weapon_dam_max_sharp)->default_value(150),
     "see configuration.h")
    ("speef_make_body",
     po::value<bool>(&speef_make_body)->default_value(false),
     "see configuration.h")
    ("nuke_repair_items",
     po::value<bool>(&nuke_repair_items)->default_value(true),
     "see configuration.h")
    ("check_multiplay",
     po::value<bool>(&check_multiplay)->default_value(true),
     "see configuration.h")
    ("force_multiplay_compliance",
     po::value<bool>(&force_multiplay_compliance)->default_value(true),
     "see configuration.h")
    ("repo_mobs",
     po::value<bool>(&repo_mobs)->default_value(false),
     "see configuration.h")
    ("super_repo_mobs",
     po::value<bool>(&super_repo_mobs)->default_value(false),
     "see configuration.h")
    ("no_damaged_items_shop",
     po::value<bool>(&no_damaged_items_shop)->default_value(false),
     "see configuration.h")
    ("auto_deletion",
     po::value<bool>(&auto_deletion)->default_value(false),
     "see configuration.h")
    ("rent_only_deletion",
     po::value<bool>(&rent_only_deletion)->default_value(false),
     "see configuration.h")
    ("nuke_inactive_mobs",
     po::value<bool>(&nuke_inactive_mobs)->default_value(false),
     "see configuration.h")
    ("load_on_death",
     po::value<bool>(&load_on_death)->default_value(true),
     "see configuration.h")
    ("throw_format_exceptions",
     po::value<bool>(&throw_format_exceptions)->default_value(true),
     "see configuration.h")
    ("no_mail",
     po::value<bool>(&no_mail)->default_value(false),
     "see configuration.h")
    ;

  // database options
  po::options_description databases("Databases");
  databases.add_options()
    ("sneezy_db", po::value<string>(&db_hosts[DB_SNEEZY]),
     "host for sneezy database")
    ("sneezybeta_db", po::value<string>(&db_hosts[DB_SNEEZYBETA]),
     "host for sneezybeta database (unused)")
    ("immortal_db", po::value<string>(&db_hosts[DB_IMMORTAL]),
     "host for immortal database")
    ("sneezyglobal_db", po::value<string>(&db_hosts[DB_SNEEZYGLOBAL]),
     "host for sneezyglobal database")
    ("sneezyprod_db", po::value<string>(&db_hosts[DB_SNEEZYPROD]),
     "host for sneezyprod database (unused)")
    ("sneezybuilder_db", po::value<string>(&db_hosts[DB_SNEEZYBUILDER]),
     "host for sneezybuilder database (unused)")
    ("wiki_mortal_db", po::value<string>(&db_hosts[DB_WIKI_MORTAL]),
     "host for mortal wiki database")
    ("wiki_builder_db", po::value<string>(&db_hosts[DB_WIKI_BUILDER]),
     "host for builder wiki database")
    ("wiki_admin_db", po::value<string>(&db_hosts[DB_WIKI_ADMIN]),
     "host for admin wiki database")
    ("forums_admin_db", po::value<string>(&db_hosts[DB_FORUMS_ADMIN]),
     "host for admin forums database")
    ;

  po::options_description cmdline_options;
  cmdline_options.add(cmdline_only).add(config).add(databases);

  po::options_description config_options;
  config_options.add(config).add(databases).add(config_only);

  po::options_description visible("Available options");
  visible.add(cmdline_only).add(config).add(databases).add(config_only);

  po::variables_map vm;

  po::store(po::command_line_parser(argc, argv).
    options(cmdline_options).run(), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << format("Usage: %s [options]\n") % argv[0];
    std::cout << visible;  
    return false;
  }

  if (vm.count("config")) {
    std::ifstream ifs(config_file.c_str());

    if (!ifs.is_open()) {
      std::cout << format("Failed to open config file '%s'\n") % config_file;
      return false;
    }

    po::store(parse_config_file(ifs, config_options), vm);
    po::notify(vm);
  }

  return true;
}


