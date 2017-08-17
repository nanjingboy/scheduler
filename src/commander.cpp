#include <iostream>
#include <cstdlib>
#include "commander.h"

using namespace std;

Commander::Commander(const string & name, const string & version):
  m_name(name),
  m_version(version) {
}

int Commander::find_option(const string & name) {
  size_t option_count = m_options.size();
  for (size_t index = 0; index < option_count; index++) {
    Option option = m_options[index];
    if (name == option.short_name || name == "--" + option.name) {
      return index;
    }
  }
  return -1;
}

int Commander::find_command(const string & name) {
  size_t command_count = m_commands.size();
  for (size_t index = 0; index < command_count; index++) {
    if (m_commands[index].name == name) {
      return index;
    }
  }
  return -1;
}

void Commander::print_help() {
  cout << endl << "Usage: " << m_name << " [command] [option]" << endl << endl;
  cout << "Options:" << endl << endl;
  size_t option_count = m_options.size();
  for (size_t index = 0; index < option_count; index++) {
    Option option = m_options[index];
    cout << "  " << option.short_name << ", --" << option.name << "  " << option.description << endl;
  }
  cout << "  -h, --help  Display this help message" << endl;
  cout << "  -V, --version  Display this application version" << endl << endl;
  size_t command_count = m_commands.size();
  if (command_count > 0) {
    cout << "Available Commands:" << endl << endl;
    for (size_t index = 0; index < command_count; index++) {
      Command command = m_commands[index];
      cout << "  " << command.name << "  " << command.description << endl;
    }
  }
}

void Commander::print_version() {
  cout << m_version << endl;
}

void Commander::print_option_error(const string & option) {
  cout << endl << "  error: unknown option '" << option << "'" << endl;
  exit(1);
}

void Commander::add_option(const struct Option & option) {
  m_options.push_back(option);
}

void Commander::add_command(const struct Command & command) {
  m_commands.push_back(command);
}

void Commander::run(int argc, const char * argv[]) {
  if (argc < 2) {
    print_help();
  } else {
    string first_arg = argv[1];
    if (first_arg.find("-") == 0) {
      if (first_arg == "-h" || first_arg == "--help") {
        print_help();
      } else if (first_arg == "-V" || first_arg == "--version") {
        print_version();
      } else {
        print_option_error(first_arg);
      }
    } else {
      int command_index = find_command(first_arg);
      if (command_index > -1) {
        map<string, string> option_values;
        if (argc >= 3) {
          for (int index = 2; index < argc; index++) {
            string arg = argv[index];
            if (arg == "-h" || arg == "--help") {
              print_help();
              return;
            }
            if (arg == "-V" || arg == "--version") {
              print_version();
              return;
            }
            if (arg.find("-") == 0) {
              int option_index = find_option(arg);
              if (option_index == -1) {
                print_option_error(arg);
              } else if (index < argc - 1) {
                string value = argv[index + 1];
                if (value.find("-") != 0) {
                  option_values[m_options[option_index].name] = value;
                }
              }
            }
          }
        }
        Command command = m_commands[command_index];
        if (command.callback) {
          command.callback(option_values);
        }
      }
    }
  }
}