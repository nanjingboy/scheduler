# scheduler
A simple tool that provides a easy way for writing and deploying cron jobs by C++

# Requirements

* [json](https://github.com/nlohmann/json)
* [boost](http://www.boost.org/)

# Getting Started

* git clone https://github.com/nanjingboy/scheduler ~/workspace/scheduler
* cd ~/workspace/scheduler
* make
* bin/scheduler init

#### Example schedule.json file

```javascript
[
  {
    "minutes": [0],
    "hours": [12],
    "daysOfTheMonth": [1, 2],
    "months": [1],
    "daysOfTheWeek": [6, 7],
    "command": "ENV=test HOME=/Users/tom balabalababala 2>> error.log"
  }
]
```
* minutes must between 1 and 59.
* hours must between 1 and 23.
* daysOfTheMonth must between 1 and 30.
* months must between 1 and 12.
* daysOfTheWeek must between 0 and 7.

* The value of minutes, hours, daysOfTheMonth, months, daysOfTheWeek can also be a string, [click to see the fromat](http://www.nncron.ru/help/EN/working/cron-format.htm)

# Usage

```shell
$ bin/scheduler

Usage: scheduler [command] [option]

Options:

  -p, --path  Path of the initial schedule file
  -u, --user  Specify the name of the user whose crontab is to be tweaked
  -h, --help  Display this help message
  -V, --version  Display this application version

Available Commands:

  init  Create an initial schedule file
  write  Write cron jobs that defined in initial schedule file to crontab
  clear  Clear cron jobs that defined in initial schedule file from crontab

```

# License

MIT