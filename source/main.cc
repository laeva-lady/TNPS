#include <print>
#include <vector>

#include "../header/Todo.hpp"

int main(int argc, char** argv) {
  // TODO(2025-09-08 09:59:51): insert timestamp to TODOs without them

  std::println("Getting files from command line");
  std::vector<std::string> files = {};
  for (int i = 0; i < argc; i++) {
    files.push_back(argv[i]);
  }

  std::println("Getting todos");
  auto todos = Todos::GetTodos(files);
  Todos::addIdToTodos(files, todos);

  for (const Todo t : todos) {
    t.print();
  }

  std::println("Putting todos to file");
  Todos::putToFile(todos);

  return 0;
}
