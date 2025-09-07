#include <vector>

#include "../header/Todo.hpp"

int main(int argc, char** argv) {
  // TODO: insert timestamp to TODOs without them

  std::vector<std::string> files = {};
  for (int i = 0; i < argc; i++) {
    files.push_back(argv[i]);
  }

  auto todos = Todos::GetTodos(files);

  for (const Todo t : todos) {
    t.print();
  }

  Todos::putToFile(todos);
  return 0;
}
