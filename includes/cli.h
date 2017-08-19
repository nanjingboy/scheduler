#ifndef CLI_H_
#define CLI_H_

#include <map>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>

class Cli {

private:
  static boost::filesystem::path parse_path(const std::map<std::string, std::string> & option_values);
  static std::string parse_user(const std::map<std::string, std::string> & option_values);
  static std::vector<std::string> parse_jobs_can_not_remove(const std::string & path, const std::string & user);
  static std::string parse_schedule_time(const std::vector<int> & times, int min_value, int max_value);
  static std::vector<std::string> parse_schedule_file(const std::string & path);
  static void write_crontab(const std::vector<std::string> & jobs, const std::string & user);

public:
  static void init(const std::map<std::string, std::string> & option_values);
  static void write(const std::map<std::string, std::string> & option_values);
  static void clear(const std::map<std::string, std::string> & option_values);
};

#endif //CLI_H_