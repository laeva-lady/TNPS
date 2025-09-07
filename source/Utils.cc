#include "../header/Utils.hpp"
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
