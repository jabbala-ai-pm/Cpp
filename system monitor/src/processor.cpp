#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::string line, value;
  float user_time = 0.0f, nice_time = 0.0f, system_time = 0.0f, ideal_time = 0.0f, iowait_time = 0.0f, irq_time = 0.0f, softirq_time = 0.0f, steal_time = 0.0f, tot_cpu_time = 0.0f, tot_ideal_wait_time = 0.0f, tot_usage_time = 0.0f;
  int count = 0;
  std::ifstream cpu_utilization_stat_file(LinuxParser::kProcDirectory +
                                         LinuxParser::kStatFilename);
  if (cpu_utilization_stat_file.is_open()) {
    std::getline(cpu_utilization_stat_file, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (count == 1) {user_time = std::stof(value);}
      if (count == 2) {nice_time = std::stof(value);}
      if (count == 3) {system_time = std::stof(value);}
      if (count == 4) {ideal_time = std::stof(value);}
      if (count == 5) {iowait_time = std::stof(value);}
      if (count == 6) {irq_time = std::stof(value);}
      if (count == 7) {softirq_time = std::stof(value);}
      if (count == 8) {steal_time = std::stof(value);}
      count++;
    }

    tot_cpu_time = user_time + nice_time + system_time + ideal_time + iowait_time + irq_time + softirq_time + steal_time;
    tot_ideal_wait_time = ideal_time + iowait_time;
    tot_usage_time = tot_cpu_time - tot_ideal_wait_time;
    return (tot_usage_time / tot_cpu_time);
  }
  return 0.0f;
}