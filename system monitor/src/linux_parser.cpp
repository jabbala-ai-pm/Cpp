#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::string line, key, value;
  float m_total=0.0f, m_free=0.0f;
  std::ifstream meminfofile(kProcDirectory + kMeminfoFilename);
  if(meminfofile.is_open()){
    while(std::getline(meminfofile, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "MemTotal:")
        m_total = std::stof(value);
      if(key == "MemFree:")
        m_free = std::stof(value);
    }
  }
  return ( (m_total - m_free) / m_total); 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::string line, system_uptime;
  std::ifstream up_time(kProcDirectory + kUptimeFilename);
  //Ref: https://access.redhat.com/documentation/en-US/Red_Hat_Enterprise_Linux/4/html/Reference_Guide/s2-proc-uptime.html
  if(up_time.is_open()){ // format 350735.47 234388.90 => system up_time and idel process time
    std::getline(up_time, line);
    std::istringstream linestream(line);
    linestream >> system_uptime;
    return std::stol(system_uptime.c_str());
  }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;
  std::ifstream total_processes_stat_file(kProcDirectory + kStatFilename);
  if(total_processes_stat_file.is_open()){
    while(std::getline(total_processes_stat_file, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "processes")
        return std::stoi(value);
    }
  }
  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key, value;
  std::ifstream total_processes_stat_file(kProcDirectory + kStatFilename);
  if(total_processes_stat_file.is_open()){
    while(std::getline(total_processes_stat_file, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "procs_running")
        return std::stoi(value);
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::string line;
  std::ifstream command_file(LinuxParser::kProcDirectory +std::to_string(pid) +LinuxParser::kCmdlineFilename);
  if (command_file.is_open()) {
    std::getline(command_file, line);
    return line;
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::string line, key, value;
  std::ifstream ram_file(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatusFilename);
  if (ram_file.is_open()) {
    while (std::getline(ram_file, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        return std::to_string(std::stoi(value) / 1024);
      }
    }
  }
  return value;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::string line, key, value, user_id;
  std::ifstream kPasswordPathFile(LinuxParser::kPasswordPath);
  if (kPasswordPathFile.is_open()) {
    while (std::getline(kPasswordPathFile, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      std::stringstream ss(key);
      for (int i = 0; i < 3; i++) {
        std::getline(ss, value, ':');
        if (value == std::to_string(pid)) {
          std::stringstream ss1(key);
          std::getline(ss1, user_id, ':');
          return user_id;
        }
      }
    }
  }
  return user_id;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::string line, key, value, user_id;
  std::ifstream kStatusFile(LinuxParser::kProcDirectory +
                                   std::to_string(pid) +
                                   LinuxParser::kStatusFilename);
  if (kStatusFile.is_open()) {
    while (std::getline(kStatusFile, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        user_id = Uid(std::stoi(value));
        return user_id;
      }
    }
  }
  return user_id;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  std::string line, value;
  int count = 0;
  long int up_time;
  std::ifstream kStatFile(LinuxParser::kProcDirectory +
                                 std::to_string(pid) +
                                 LinuxParser::kStatFilename);
  if (kStatFile.is_open()) {
    while (std::getline(kStatFile, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        count++;
        if (count == 22) {
          up_time = std::stol(value);
          up_time = up_time / sysconf(_SC_CLK_TCK);
        }
      }
    }
  }
  return up_time;
}