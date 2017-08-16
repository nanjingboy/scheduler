#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#ifdef __linux__
#include <linux/limits.h>
#else
#include <limits.h>
#endif

#include "commander.h"

using namespace std;

string parse_path(const map<string, string> & option_values) {
  if (option_values.find("path") == option_values.end()) {
    char current_path[PATH_MAX];
    getcwd(current_path, sizeof(current_path));
    return string(current_path);
  }
  return option_values.at("path");
}

string parse_user(const map<string, string> & option_values) {
  if (option_values.find("user") == option_values.end()) {
    passwd *pwd;
    pwd = getpwuid(geteuid());
    char current_path[PATH_MAX];
    getcwd(current_path, sizeof(current_path));
    return string(pwd->pw_name);
  }
  return option_values.at("user");
}

void init(const map<string, string> & option_values) {
  cout << "call init..." << endl;
  cout << "path:" << parse_path(option_values) << endl;
  cout << "user:" << parse_user(option_values) << endl;
}

void write(const map<string, string> & option_values) {
  cout << "call write..." << endl;
  cout << "path:" << parse_path(option_values) << endl;
  cout << "user:" << parse_user(option_values) << endl;
}

void clear(const map<string, string> & option_values) {
  cout << "call clear..." << endl;
  cout << "path:" << parse_path(option_values) << endl;
  cout << "user:" << parse_user(option_values) << endl;
}

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
  init_command.callback = init;

  Command write_command;
  write_command.name = "write";
  write_command.description = "Write cron jobs that defined in schedule.cpp file to crontab";
  write_command.callback = write;

  Command clear_command;
  clear_command.name = "clear";
  clear_command.description = "Clear cron jobs that defined in schedule.cpp file from crontab";
  clear_command.callback = clear;

  Commander commander("scheduler", "0.1.0");
  commander.add_option(path_option);
  commander.add_option(user_option);
  commander.add_command(init_command);
  commander.add_command(write_command);
  commander.add_command(clear_command);
  commander.run(argc, argv);
  return 0;
}