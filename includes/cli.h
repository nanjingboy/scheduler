#ifndef CLI_H_
#define CLI_H_

#include <map>
#include <string>
#include <boost/filesystem.hpp>

class Cli {

private:
  static boost::filesystem::path parse_path(const std::map<std::string, std::string> & option_values);
  static std::string parse_user(const std::map<std::string, std::string> & option_values);

public:
  static void init(const std::map<std::string, std::string> & option_values);
  static void write(const std::map<std::string, std::string> & option_values);
  static void clear(const std::map<std::string, std::string> & option_values);
};

#endif //CLI_H_