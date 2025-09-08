#include <print>
#include <vector>

#include "../header/Todo.hpp"

int main(int argc, char** argv) {
  std::string pattern = argv[1];
  std::vector<std::string> files = {};
  for (int i = 2; i < argc; i++) {
    files.push_back(argv[i]);
  }

  Issues issues(pattern, files);
  issues.print();
  issues.putToFile("issues.txt");

  return 0;
}
