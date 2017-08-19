#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <boost/process.hpp>
#include <json.hpp>

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

vector<string> Cli::parse_jobs_can_not_remove(const string & path, const string & user) {
  vector<string> jobs;
  process::ipstream pipe_stream;
  process::system("sudo crontab -l -u " + user, process::std_out > pipe_stream, process::std_err > process::null);

  string job;
  bool removeable = false;
  while (pipe_stream && getline(pipe_stream, job) && !job.empty()) {
    string start_comment = "#BEGIN DEFINE CRON JOBS FROM:" + path;
    if (job == start_comment) {
      removeable = true;
      continue;
    }

    string end_comment = "#END DEFINE CRON JOBS FROM:" + path;
    if (job == end_comment) {
      removeable = false;
      continue;
    }
    if (!removeable) {
      jobs.push_back(job);
    }
  }
  return jobs;
}

string Cli::parse_schedule_time(const vector<int> & times, int min_value, int max_value) {
  string result = "";
  for (int time: times) {
    if (time >= min_value && time <= max_value) {
      result += to_string(time) + ",";
    }
  }
  if (result != "") {
    result.pop_back();
    return result;
  }

  return "*";
}

vector<string> Cli::parse_schedule_file(const string & path) {
  vector<string> jobs;
  ifstream ifs(path);
  if (ifs.is_open()) {
    nlohmann::json json;
    ifs >> json;
    ifs.close();
    if (json.is_array()) {
      if (json.size() > 0) {
        for (auto & element: json) {
          string command = "";
          if (element["command"].is_string()) {
            command = element["command"];
          }
          if (command == "") {
            continue;
          }
          string sections[] = { "*", "*", "*", "*", "*" };
          if (element["minutes"].is_string()) {
            sections[0] = element["minutes"];
          } else if (element["minutes"].is_array()) {
            sections[0] = parse_schedule_time(element["minutes"], 0, 59);
          }

          if (element["hours"].is_string()) {
            sections[1] = element["hours"];
          } else if (element["hours"].is_array()) {
            sections[1] = parse_schedule_time(element["hours"], 0, 23);
          }

          if (element["daysOfTheMonth"].is_string()) {
            sections[2] = element["daysOfTheMonth"];
          } else if (element["daysOfTheMonth"].is_array()) {
            sections[2] = parse_schedule_time(element["daysOfTheMonth"], 1, 31);
          }

          if (element["months"].is_string()) {
            sections[3] = element["months"];
          } else if (element["months"].is_array()) {
            sections[3] = parse_schedule_time(element["months"], 1, 12);
          }

          if (element["daysOfTheWeek"].is_string()) {
            sections[4] = element["daysOfTheWeek"];
          } else if (element["daysOfTheWeek"].is_array()) {
            sections[4] = parse_schedule_time(element["daysOfTheWeek"], 0, 7);
          }

          string job = "";
          for (const string & section: sections) {
            job += section + " ";
          }
          job += " " + command;
          jobs.push_back(job);
        }
      }
    } else {
      cout << "Schedule file format is error: " << path << endl;
      exit(1);
    }
  }
  return jobs;
}

void Cli::write_crontab(const vector<string> & jobs, const string & user) {
  if (jobs.size() == 0) {
    process::system("sudo crontab -r -u " + user, process::std_err > process::null);
    return;
  }
  filesystem::path crontab_tmp_path = filesystem::temp_directory_path() /= "crontab.tmp";
  ofstream ofs(crontab_tmp_path.string());
  if (ofs.is_open()) {
    for (const string & job: jobs) {
      ofs << job << endl;
    }
    ofs.close();
  }
  process::system("sudo crontab -u " + user + " " + crontab_tmp_path.string());
  filesystem::remove(crontab_tmp_path);
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

  ofstream ofs(path_string);
  if (ofs.is_open()) {
    ofs << "[" << endl << "]" << endl;
    ofs.close();
  }
  cout << "Initial file created: " << path_string << endl;
}

void Cli::write(const map<string, string> & option_values) {
  vector<string> schedule_file_jobs;
  filesystem::path path = parse_path(option_values);
  string path_string = filesystem::weakly_canonical(path).string();
  if (filesystem::exists(path)) {
    schedule_file_jobs = parse_schedule_file(path_string);
    if (schedule_file_jobs.size() > 0) {
      schedule_file_jobs.insert(schedule_file_jobs.begin(), "#BEGIN DEFINE CRON JOBS FROM:" + path_string);
      schedule_file_jobs.push_back("#END DEFINE CRON JOBS FROM:" + path_string);
    }
  }

  string user = parse_user(option_values);
  vector<string> jobs = parse_jobs_can_not_remove(path_string, user);
  for (const string & job: schedule_file_jobs) {
    jobs.push_back(job);
  }
  write_crontab(jobs, user);
  cout << "Crontab file has been written" << endl;
}

void Cli::clear(const map<string, string> & option_values) {
  string user = parse_user(option_values);
  string path = filesystem::weakly_canonical(parse_path(option_values)).string();
  write_crontab(parse_jobs_can_not_remove(path, user), user);
  cout << "Crontab file has been cleared" << endl;
}