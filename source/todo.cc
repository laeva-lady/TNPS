#include "../header/Todo.hpp"

#include <fstream>
#include <print>
#include <regex>
#include <string>

std::vector<Todo> Todos::GetTodos(const std::vector<std::string>& files) {
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
  return todos;
}
void Todos::putToFile(std::vector<Todo> todos) {
  // TODO(2025-09-08 10:03:00): change to JSON ?
  std::ofstream Output("todos.txt");

  std::string str = "";
  for (const Todo t : todos) {
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

void Todos::print(std::vector<Todo> todos) {
  for (const Todo t : todos) {
    t.print();
  }
}

void Todos::addIdToTodos(const std::vector<std::string>& files, std::vector<Todo> todos) {
  for (const auto& todo : todos) {
    if (todo.timestamp.empty()) {
      std::ifstream file_in(todo.filepath);
      std::vector<std::string> lines;
      std::string line;
      while (std::getline(file_in, line)) {
        lines.push_back(line);
      }
      file_in.close();

      std::string& line_to_modify = lines[todo.linenumber - 1];
      std::regex todo_pattern(R"(TODO:)");
      std::string timestamp = get_timestamp();
      line_to_modify = std::regex_replace(line_to_modify, todo_pattern, "TODO(" + timestamp + "):");

      std::ofstream file_out(todo.filepath);
      for (const auto& l : lines) {
        file_out << l << std::endl;
      }
      file_out.close();
      std::println("Inserting timestamp <{}> to <{}> @ <{}>:<{}>", timestamp, todo.title, todo.filepath,
                   todo.linenumber);
    }
  }
}
