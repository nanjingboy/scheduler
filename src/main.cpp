#include <iostream>
#include "cli.h"
#include "commander.h"

int main(int argc, const char * argv[]) {
  Option path_option;
  path_option.short_name = "-p";
  path_option.name = "path";
  path_option.description = "Path of the initial schedule.cpp file";

  Option user_option;
  user_option.short_name = "-u";
  user_option.name = "user";
  user_option.description = "Specify the name of the user whose crontab is to be tweaked";

  Command init_command;
  init_command.name = "init";
  init_command.description = "Create an initial schedule.cpp file";
  init_command.callback = Cli::init;

  Command write_command;
  write_command.name = "write";
  write_command.description = "Write cron jobs that defined in schedule.cpp file to crontab";
  write_command.callback = Cli::write;

  Command clear_command;
  clear_command.name = "clear";
  clear_command.description = "Clear cron jobs that defined in schedule.cpp file from crontab";
  clear_command.callback = Cli::clear;

  Commander commander("scheduler", "0.1.0");
  commander.add_option(path_option);
  commander.add_option(user_option);
  commander.add_command(init_command);
  commander.add_command(write_command);
  commander.add_command(clear_command);
  commander.run(argc, argv);
  return 0;
}