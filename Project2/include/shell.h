#pragma once
#include <map>
#include <string>
#include <vector>
#include <optional>
#include <memory>
#include "vision/vision.h"

class Shell {
public:
  Shell();
  void run();

private:
  std::string get_prompt();
  bool handle_builtin(const std::string &line);
  void setup_signals();
  void load_aliases();
  void load_todo();
  void save_todo();
  void load_config();
  void save_config();
  void show_dashboard();
  void nova_intelligence(const std::string &query);
  std::string call_gemini_api(const std::string &prompt);
  std::string resolve_alias(const std::string &cmd);
  std::string expand_variables(const std::string &arg);
  void suggest_command(const std::string &cmd);
  void update_dir_history(const std::string &path);
  int levenshtein_distance(const std::string &s1, const std::string &s2);
  void vision_command(const std::string &args);

  int last_status = 0;
  double last_exec_time = 0.0;
  std::map<std::string, std::string> aliases;
  std::map<std::string, int> dir_frequency;
  std::vector<std::string> todo_list;
  std::vector<std::string> command_history;
  std::string model_name = "gemini-2.0-flash";
  std::string api_version = "v1";
  std::string access_token = "";
  std::unique_ptr<novashell::vision::SeeCommand> vision_command_;
};
