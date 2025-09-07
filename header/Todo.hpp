#pragma once

#include <print>
#include <string>

#include "../header/Utils.hpp"

struct Todo {
  std::string timestamp;  // the timestamp of the todo
  std::string title;      // the timestamp of the todo
  std::string text;       // string separated by \n
  std::string filepath;
  int linenumber;

  // TODO: maybe rework this to make it a lil bit nicer???

  Todo(std::string id, int linenumber, std::string title, std::string text, std::string filepath) {
    this->timestamp = id;
    this->title = title;
    this->text = text;
    this->filepath = filepath;
    this->linenumber = linenumber;
  }

  void print() const {
    std::println("file: <{}>", this->filepath);
    std::println("timestamp: <{}>", this->timestamp);
    std::println("line: <{}>", this->linenumber);
    std::println("title: <{}>", this->title);
    std::println("text:");
    auto lines = slit_string(this->text);
    for (auto line : lines) {
      std::println("\t<{}>", line);
    }

    std::println();
  }
};

namespace Todos {
std::vector<Todo> GetTodos(const std::vector<std::string>& files);
void putToFile(std::vector<Todo> todos);
void print(std::vector<Todo> todos);
};  // namespace Todos
