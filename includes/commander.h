#ifndef COMMANDER_H_
#define COMMANDER_H_

#include <map>
#include <vector>
#include <string>

typedef void (*command_callback)(const std::map<std::string, std::string> &);

struct Option {
  std::string short_name;
  std::string name;
  std::string description;
};

struct Command {
  std::string name;
  std::string description;
  command_callback callback;
};

class Commander {

private:
  std::string m_name;
  std::string m_version;
  std::vector<Option> m_options;
  std::vector<Command> m_commands;

  int find_option(const std::string & name);
  int find_command(const std::string & name);

  void print_help();
  void print_version();
  void print_option_error(const std::string & option);

public:
  Commander(const std::string & name, const std::string & version);
  void add_option(const struct Option & option);
  void add_command(const struct Command & command);
  void run(int argc, const char * argv[]);
};

#endif //COMMANDER_H_