#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 
  std::string line, value;
  int count = 0;
  float utime, stime, cutime, cstime, starttime, uptime, total_time, seconds, m_cpu_utilization;
  std::ifstream file_for_cpu_uilization(LinuxParser::kProcDirectory + std::to_string(pid_) + LinuxParser::kStatFilename);
  if (file_for_cpu_uilization.is_open()) {
    while (std::getline(file_for_cpu_uilization, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        count++;
        if (count == 13) { utime = std::stol(value); }
        if (count == 14) { stime = std::stol(value); }
        if (count == 15) { cutime = std::stol(value);}
        if (count == 16) { cstime = std::stol(value);}
        if (count == 21) { starttime = std::stol(value);}
        uptime = UpTime();
        total_time = utime + stime + cutime + cstime;
        seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
        if (seconds == 0) {return 0.0f;}
        m_cpu_utilization =(((total_time / sysconf(_SC_CLK_TCK)) / seconds));
      }
    }
  }
  return m_cpu_utilization;
}

// DONE: Return the command that generated this process
string Process::Command() {
  return LinuxParser::Command(pid_);
}

// DONE: Return this process's memory utilization
string Process::Ram() { 
  return LinuxParser::Ram(pid_); 
}

// DONE: Return the user (name) that generated this process
string Process::User() { 
  return LinuxParser::User(pid_); 
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { 
  return LinuxParser::UpTime(pid_); 
}

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }