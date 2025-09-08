#pragma once

#include <fstream>
#include <print>
#include <regex>
#include <string>
#include <vector>

#include "../header/Utils.hpp"

struct Issue {
  std::string timestamp;  // the timestamp of the todo
  std::string title;      // the timestamp of the todo
  std::string text;       // string separated by \n
  std::string filepath;
  int linenumber;

  Issue(std::string id, int linenumber, std::string title, std::string text, std::string filepath) {
    this->timestamp = id;
    this->title = title;
    this->text = text;
    this->filepath = filepath;
    this->linenumber = linenumber;
  }
};

template <>
struct std::formatter<Issue> : std::formatter<std::string> {
  auto format(const Issue& p, auto& ctx) const {
    std::string str;
    str += std::format("file: <{}>\n", p.filepath);
    str += std::format("timestamp: <{}>\n", p.timestamp);
    str += std::format("line: <{}>\n", p.linenumber);
    str += std::format("title: <{}>\n", p.title);
    str += std::format("text:\n");
    auto lines = slit_string(p.text);
    for (auto line : lines) {
      str += std::format("\t<{}>\n", line);
    }

    str += '\n';
    return std::formatter<std::string>::format(str, ctx);
  }
};

class Issues {
 public:
  Issues(std::string pattern, std::vector<std::string> files) {
    this->pattern = pattern;
    this->files = files;

    GetIssues();
    addIdToIssues();
  }

  void print() {
    for (const Issue t : this->issues) {
      std::println("{}", t);
    }
  }

  void putToFile(std::string output_file) {
    std::ofstream Output(output_file);

    std::string str = "";
    for (const Issue t : this->issues) {
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

 private:
  std::vector<Issue> issues;
  std::string pattern = "TODO";
  std::vector<std::string> files;

  void GetIssues() {
    const std::regex todo_pattern(this->pattern + R"((?:\(([^)]*)\))?: )");

    for (const auto& f : this->files) {
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
            this->issues.push_back(Issue(current_id, todo_linenumber, title, block, f));
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
          this->issues.push_back(Issue(current_id, todo_linenumber, title, block, f));
          block.clear();
          current_id.clear();
          in_todo_block = false;
        }
      }

      if (in_todo_block && !block.empty()) {
        this->issues.push_back(Issue(current_id, todo_linenumber, title, block, f));
      }
    }

    std::sort(issues.begin(), issues.end(), [](const Issue& a, const Issue& b) {
      return a.timestamp < b.timestamp;  // ascending
    });
  }

  void addIdToIssues() {
    for (auto& issue : this->issues) {
      if (issue.timestamp.empty()) {
        std::ifstream file_in(issue.filepath);
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file_in, line)) {
          lines.push_back(line);
        }
        file_in.close();

        std::string& line_to_modify = lines[issue.linenumber - 1];
        std::regex regex_pattern(this->pattern + R"((?:\(\))?:)");
        issue.timestamp = get_timestamp();
        line_to_modify =
            std::regex_replace(line_to_modify, regex_pattern, this->pattern + "(" + issue.timestamp + "):");

        std::ofstream file_out(issue.filepath);
        for (const auto& l : lines) {
          file_out << l << std::endl;
        }
        file_out.close();
        std::println("Inserting timestamp <{}> to <{}> @ <{}>:<{}>", issue.timestamp, issue.title, issue.filepath,
                     issue.linenumber);
      }
    }
  }
};
