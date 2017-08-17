#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "cli.h"

using namespace std;
using namespace boost;

filesystem::path Cli::parse_path(const map<string, string> & option_values) {
  if (option_values.find("path") == option_values.end()) {
    return (filesystem::current_path() /= "schedule.json");
  }
  return filesystem::absolute(option_values.at("path"));
}

string Cli::parse_user(const map<string, string> & option_values) {
  if (option_values.find("user") == option_values.end()) {
    passwd *pwd;
    pwd = getpwuid(geteuid());
    return pwd->pw_name;
  }
  return option_values.at("user");
}

void Cli::init(const map<string, string> & option_values) {
  filesystem::path path = parse_path(option_values);
  string path_string = filesystem::weakly_canonical(path).string();

  if (filesystem::is_directory(path)) {
    cout << "Initial file can't be directory: " << path_string << endl;
    exit(1);
  }

  if (filesystem::exists(path)) {
    cout << "Initial file already exists: " << path_string << endl;
    exit(1);
  }

  filesystem::path parent_path = path.parent_path();
  if (!filesystem::is_directory(parent_path)) {
    cout << "Directory does not exist: " << filesystem::weakly_canonical(parent_path).string() << endl;
    exit(1);
  }

  ofstream ofs { path_string };
  ofs << "[" << endl << "]" << endl;
  ofs.close();
  cout << "Initial file created: " << path_string << endl;
}

void Cli::write(const map<string, string> & option_values) {
  cout << "call write..." << endl;
  cout << "path:" << parse_path(option_values).string() << endl;
  cout << "user:" << parse_user(option_values) << endl;
}

void Cli::clear(const map<string, string> & option_values) {
  cout << "call clear..." << endl;
  cout << "path:" << parse_path(option_values).string() << endl;
  cout << "user:" << parse_user(option_values) << endl;
}