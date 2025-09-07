#pragma once

#include <fstream>
#include <print>
#include <regex>
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

class Todos {
 public:
  std::vector<Todo> todos;

  Todos(const std::vector<std::string>& files) {
    std::vector<Todo> todos;

    static const std::regex todo_pattern(R"(TODO(?:\(([^)]*)\))?: )");

    for (const auto& f : files) {
      std::ifstream MyReadFile(f);
      std::string line;
      std::string prefix;
      std::string block;
      bool in_todo_block = false;

      std::string title;
      std::string current_id;
      int todo_linenumber = 0;

      int linenumber = 0;

      while (std::getline(MyReadFile, line)) {
        ++linenumber;

        std::smatch match;
        if (std::regex_search(line, match, todo_pattern)) {
          // If we were in a previous TODO block, finalize it first
          if (in_todo_block) {
            todos.push_back(Todo(current_id, todo_linenumber, title, block, f));
            block.clear();
            current_id.clear();
          }

          // Start new TODO
          prefix = line.substr(0, match.position());
          title = line.substr(match.position() + match.length(), line.length());
          todo_linenumber = linenumber;
          in_todo_block = true;
          if (match.size() > 1 && !match[1].str().empty()) {
            current_id = match[1].str();
          }
        } else if (in_todo_block && line.starts_with(prefix)) {
          // Continuation line of current TODO
          block += line.substr(prefix.length(), line.length()) + "\n";
        } else if (in_todo_block) {
          // End of TODO block
          if (!block.empty() && block.back() == '\n') {
            block.pop_back();
          }
          todos.push_back(Todo(current_id, todo_linenumber, title, block, f));
          block.clear();
          current_id.clear();
          in_todo_block = false;
        }
      }

      if (in_todo_block && !block.empty()) {
        todos.push_back(Todo(current_id, todo_linenumber, title, block, f));
      }
    }

    std::sort(todos.begin(), todos.end(), [](const Todo& a, const Todo& b) {
      return a.timestamp < b.timestamp;  // ascending
    });
    this->todos = todos;
  }

  void putToFile() {
    // TODO: change to JSON ?
    std::ofstream Output("todos.txt");

    std::string str = "";
    for (const Todo t : this->todos) {
      std::string separator = ";";
      str += t.timestamp + separator;
      str += t.filepath + ":" + std::to_string(t.linenumber) + separator;
      str += t.title + separator;
      str += t.text;
      str += "\n";
    }

    Output << str;
    Output.close();
  }

  void print() {
    for (const Todo t : this->todos) {
      t.print();
    }
  }
};
