#include <vector>

#include "../header/Todo.hpp"

int main(int argc, char** argv) {
  // TODO: Get the files from cli args
  // TODO: Get TODOs immediatly following another to be a separate entry
  // TODO: insert timestamp to TODOs without them

  std::vector<std::string> files = {};
  for (int i = 0; i < argc; i++) {
    files.push_back(argv[i]);
  }

  Todos* todos = new Todos(files);
  for (const Todo t : todos->todos) {
    t.print();
  }

  todos->putToFile();
  return 0;
}
