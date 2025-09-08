#include "../header/Utils.hpp"

#include <chrono>
#include <format>
#include <sstream>

std::vector<std::string> slit_string(std::string text) {
  std::vector<std::string> strings;
  std::istringstream f(text);
  std::string s;
  while (getline(f, s, '\n')) {
    strings.push_back(s);
  }
  return strings;
}

std::string get_timestamp() {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
  return ss.str();
}
