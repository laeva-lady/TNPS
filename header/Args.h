#pragma once

#include <expected>
#include <fstream>
#include <print>
#include <string>
#include <tuple>
#include <vector>

struct Arguments {
  std::string pattern = "TODO";
  std::vector<std::string> files;
  std::vector<std::string> invalids;
};

template <>
struct std::formatter<Arguments> : std::formatter<std::string> {
  auto format(const Arguments& arg, auto& ctx) const {
    std::string str = std::format("({}) : {}\n\t{}", arg.pattern, arg.files, arg.invalids);
    return std::formatter<std::string>::format(str, ctx);
  }
};

namespace Args {
std::expected<Arguments, std::string> parse(int argc, char**& argv);
}
