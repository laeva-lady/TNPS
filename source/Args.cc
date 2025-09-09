#include "../header/Args.h"

#include <exception>
#include <filesystem>
#include <print>

std::expected<Arguments, std::string> Args::parse(int argc, char**& argv) {
  // argv is separated by spaces...
  //
  // -f | everything after it are files (or until --)
  // -p | pattern to search, defaults to `TODO`
  //
  struct Arguments argsss;

  bool in_files = false;
  bool found_pattern = false;
  for (int i = 1; i < argc; i++) {
    std::string current_word = argv[i];

    if (current_word == "-p" && !found_pattern) {
      i += 1;
      if (i >= argc) return std::unexpected("No value after -p");
      found_pattern = true;
      argsss.pattern = argv[i];
    } else if (current_word == "-f" && !in_files) {
      in_files = true;
      i += 1;  // Move past the -f flag
      while (i < argc) {
        std::string current_file = argv[i];
        if (current_file == "--") {
          in_files = false;
          break;
        }
        if (std::filesystem::exists(current_file)) {
          argsss.files.push_back(current_file);
        } else {
          argsss.invalids.push_back(current_file);
        }
        i++;
      }
    } else if (!in_files) {
      argsss.invalids.push_back(current_word);
    }
  }

  return argsss;
}
