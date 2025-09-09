#include <print>
#include <vector>

#include "../header/Args.h"
#include "../header/Todo.hpp"

int main(int argc, char** argv) {
  // TODO(2025-09-08 21:51:29): Make a flag to toggle printing to stdout/file

  auto results = Args::parse(argc, argv); // TODO(2025-09-08 21:51:29): check if todos at end of line works????

  if (results.has_value()) {
    Arguments arg = results.value();
    std::println("{}", arg);

    Issues issues(arg.pattern, arg.files);
    issues.print();
    issues.putToFile("issues.txt");
  } else {
    std::println("{}", results.error());
  }

  return 0;
}
