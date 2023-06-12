#include <string>
#include <iomanip>

#include "format.h"

using std::string;

std::string Format::ElapsedTime(long seconds) {
  std::ostringstream hh, mm, ss;
  hh << std::setw(2) << std::setfill('0') << std::to_string((seconds / 3600));
  mm << std::setw(2) << std::setfill('0') << std::to_string((seconds % 3600 ) / 60);
  ss << std::setw(2) << std::setfill('0') << std::to_string((seconds % 3600) % 60);
  return hh.str() + ":" + mm.str() + ":" + ss.str(); 
}