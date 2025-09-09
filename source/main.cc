#include <print>
#include <vector>

#include "../header/Args.h"
#include "../header/Todo.hpp"

int main(int argc, char** argv) {
  auto results = Args::parse(argc, argv);

  if (results.has_value()) {
    Arguments arg = results.value();
    std::println("Using this pattern: {}, on these files: {}", arg.pattern, arg.files);
    std::println("these were invalid arguments: {}", arg.invalids);

    Issues issues(arg.pattern, arg.files);
    issues.print();
    issues.putToFile("issues.txt");
  } else {
    std::println("{}", results.error());
  }

  return 0;
}
