#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#ifdef __linux__
#include <linux/limits.h>
#else
#include <limits.h>
#endif

#include "cli.h"

using namespace std;

string Cli::parse_path(const map<string, string> & option_values) {
  if (option_values.find("path") == option_values.end()) {
    char current_path[PATH_MAX];
    getcwd(current_path, sizeof(current_path));
    return current_path;
  }
  return option_values.at("path");
}

string Cli::parse_user(const map<string, string> & option_values) {
  if (option_values.find("user") == option_values.end()) {
    passwd *pwd;
    pwd = getpwuid(geteuid());
    char current_path[PATH_MAX];
    getcwd(current_path, sizeof(current_path));
    return pwd->pw_name;
  }
  return option_values.at("user");
}

void Cli::init(const map<string, string> & option_values) {
  cout << "call init..." << endl;
  cout << "path:" << parse_path(option_values) << endl;
  cout << "user:" << parse_user(option_values) << endl;
}

void Cli::write(const map<string, string> & option_values) {
  cout << "call write..." << endl;
  cout << "path:" << parse_path(option_values) << endl;
  cout << "user:" << parse_user(option_values) << endl;
}

void Cli::clear(const map<string, string> & option_values) {
  cout << "call clear..." << endl;
  cout << "path:" << parse_path(option_values) << endl;
  cout << "user:" << parse_user(option_values) << endl;
}