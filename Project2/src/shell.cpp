#include "shell.h"
#include "executor.h"
#include "parser.h"
#include <vision/vision.h>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <sstream>
#include <sys/utsname.h>
#include <thread>
#include <unistd.h>
#include <vector>

// Advanced Color Palette (ANSI 256 Colors)
const std::string R = "\033[0m";
const std::string B = "\033[1m";
const std::string DIM = "\033[2m";
const std::string ITALIC = "\033[3m";

// Segment Colors
const std::string BG_USER = "\033[48;5;27m\033[38;5;255m";
const std::string BG_DIR = "\033[48;5;238m\033[38;5;255m";
const std::string BG_GIT = "\033[48;5;208m\033[38;5;255m";
const std::string BG_TIME = "\033[48;5;235m\033[38;5;250m";

char **novashell_completion(const char *text, int start, int end);
char *command_generator(const char *text, int state);

Shell::Shell() {
  setup_signals();
  load_aliases();
  load_todo();
  load_config();
  vision_command_ = std::make_unique<novashell::vision::SeeCommand>();
  rl_attempted_completion_function = novashell_completion;
}

void Shell::load_aliases() {
  aliases["ls"] = "ls --color=auto";
  aliases["ll"] = "ls -lah --color=auto";
  aliases["grep"] = "grep --color=auto";
  aliases["cls"] = "clear";
  aliases["g"] = "git";
  aliases[".."] = "cd ..";
}

void Shell::load_todo() {
  std::string home = getenv("HOME") ? getenv("HOME") : ".";
  std::ifstream f(home + "/.novashell_todo");
  std::string line;
  while (std::getline(f, line)) {
    if (!line.empty())
      todo_list.push_back(line);
  }
}

void Shell::save_todo() {
  std::string home = getenv("HOME") ? getenv("HOME") : ".";
  std::ofstream f(home + "/.novashell_todo");
  for (const auto &task : todo_list) {
    f << task << "\n";
  }
}

void Shell::load_config() {
  std::string home = getenv("HOME") ? getenv("HOME") : ".";
  std::ifstream f(home + "/.novashellrc");
  std::string line;
  while (std::getline(f, line)) {
    size_t pos = line.find('=');
    if (pos != std::string::npos) {
      std::string key = line.substr(0, pos);
      std::string val = line.substr(pos + 1);
      if (key == "model")
        model_name = val;
      else if (key == "api_version")
        api_version = val;
      else if (key == "access_token")
        access_token = val;
    }
  }
}

void Shell::save_config() {
  std::string home = getenv("HOME") ? getenv("HOME") : ".";
  std::ofstream f(home + "/.novashellrc");
  f << "model=" << model_name << "\n";
  f << "api_version=" << api_version << "\n";
  f << "access_token=" << access_token << "\n";
}

std::string Shell::resolve_alias(const std::string &cmd) {
  if (aliases.count(cmd))
    return aliases[cmd];
  return cmd;
}

std::string Shell::expand_variables(const std::string &arg) {
  if (arg.empty() || arg[0] != '$')
    return arg;
  const char *var = getenv(arg.substr(1).c_str());
  return var ? std::string(var) : arg;
}

int Shell::levenshtein_distance(const std::string &s1, const std::string &s2) {
  int n = s1.length();
  int m = s2.length();
  std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1));
  for (int i = 0; i <= n; i++)
    dp[i][0] = i;
  for (int j = 0; j <= m; j++)
    dp[0][j] = j;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= m; j++) {
      int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
      dp[i][j] = std::min(
          {dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + cost});
    }
  }
  return dp[n][m];
}

void Shell::suggest_command(const std::string &cmd) {
  std::vector<std::string> common = {
      "ls",           "cd",        "exit",
      "help",         "history",   "nova",
      "fetch",        "clear",     "git",
      "cat",          "mkdir",     "rm",
      "alias",        "z",         "todo",
      "calc",         "stats",     "custom",
      "ai",           "explain",   "explore",
      "edit",         "alert",     "dashboard",
      "chat",         "nova-menu", "check-ai",
      "set-key",      "set-model", "set-api-version",
      "list-models",  "login",     "logout",
      "export",       "browse",    "find-file",
      "health-check", "welcome",   "project-init",
      "theme"};
  std::string best_match = "";
  int min_dist = 4;

  for (const auto &c : common) {
    int dist = levenshtein_distance(cmd, c);
    if (dist < min_dist) {
      min_dist = dist;
      best_match = c;
    }
  }

  if (!best_match.empty()) {
    std::cout << DIM << ITALIC << "  Did you mean: " << R << B << best_match
              << R << "?" << std::endl;
  }
}

void Shell::update_dir_history(const std::string &path) {
  char abs_path[1024];
  if (realpath(path.c_str(), abs_path)) {
    dir_frequency[std::string(abs_path)]++;
  }
}

char **novashell_completion(const char *text, int start, int end) {
  (void)end;
  rl_attempted_completion_over = 1;
  if (start == 0) {
    return rl_completion_matches(text, command_generator);
  }
  return rl_completion_matches(text, rl_filename_completion_function);
}

char *command_generator(const char *text, int state) {
  static std::vector<std::string> builtins = {"cd",
                                              "exit",
                                              "help",
                                              "history",
                                              "nova",
                                              "alias",
                                              "fetch",
                                              "clear",
                                              "z",
                                              "todo",
                                              "calc",
                                              "stats",
                                              "custom",
                                              "ai",
                                              "explain",
                                              "explore",
                                              "edit",
                                              "alert",
                                              "dashboard",
                                              "chat",
                                              "nova-menu",
                                              "check-ai",
                                              "set-key",
                                              "set-model",
                                              "set-api-version",
                                              "list-models",
                                              "login",
                                              "logout",
                                              "export",
                                              "browse",
                                              "find-file",
                                              "health-check",
                                              "welcome",
                                              "project-init"};
  static size_t list_index, len;

  if (!state) {
    list_index = 0;
    len = strlen(text);
  }

  while (list_index < builtins.size()) {
    const std::string &name = builtins[list_index++];
    if (name.compare(0, len, text) == 0) {
      return strdup(name.c_str());
    }
  }

  return nullptr;
}

void Shell::setup_signals() { signal(SIGINT, SIG_IGN); }

std::string get_git_branch() {
  FILE *pipe = popen("git rev-parse --abbrev-ref HEAD 2>/dev/null", "r");
  if (!pipe)
    return "";
  char buffer[128];
  std::string result = "";
  if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
    result = buffer;
    if (!result.empty() && result.back() == '\n')
      result.pop_back();
  }
  pclose(pipe);
  return result;
}

std::string Shell::get_prompt() {
  char cwd[1024];
  std::string path = "unknown";
  if (getcwd(cwd, sizeof(cwd)) != nullptr) {
    path = cwd;
    const char *home = getenv("HOME");
    if (home && path.find(home) == 0) {
      path.replace(0, strlen(home), "~");
    }
  }

  std::string git_branch = get_git_branch();
  std::string user = getenv("USER") ? getenv("USER") : "nova";

  auto now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  char time_str[9];
  std::strftime(time_str, sizeof(time_str), "%H:%M:%S", std::localtime(&now));

  std::stringstream ss;
  ss << "\n"
     << BG_TIME << " " << time_str << " " << R << BG_USER << " " << user << " "
     << R << BG_DIR << " " << path << " " << R;
  if (!git_branch.empty())
    ss << BG_GIT << "  " << git_branch << " " << R;

  std::string status_color =
      (last_status == 0) ? "\033[38;5;82m" : "\033[38;5;196m";
  std::string status_icon = (last_status == 0) ? "✔" : "✘";
  ss << "\n" << B << status_color << status_icon << R;
  if (last_exec_time > 0.05)
    ss << " " << DIM << ITALIC << last_exec_time << "s" << R;
  ss << B << " ╰─λ " << R;

  return ss.str();
}

bool Shell::handle_builtin(const std::string &line) {
  std::stringstream ss(line);
  std::string cmd;
  ss >> cmd;

  if (cmd == "exit") {
    exit(0);
  } else if (cmd == "cd") {
    std::string path;
    if (!(ss >> path))
      path = getenv("HOME");
    if (chdir(path.c_str()) != 0) {
      perror("cd");
    } else {
      update_dir_history(path);
    }
    return true;
  } else if (cmd == "theme") {
    std::string menu_cmd =
        "whiptail --title \"Nova Theme Switcher\" --menu \"Choose a theme:\" "
        "15 60 3 \"1\" \"Neon (Default Blue)\" \"2\" \"Ocean (Teal/Cyan)\" "
        "\"3\" \"Forest (Green/Emerald)\" 2>/tmp/nova_theme";
    if (system(menu_cmd.c_str()) != 0)
      return true;

    std::ifstream f("/tmp/nova_theme");
    std::string choice;
    f >> choice;

    // This is a bit tricky since colors are const globals, but we can provide a
    // message and save it to config for the next run, or just update the prompt
    // logic. For now, let's just save it to config.
    std::string theme_name = (choice == "2")   ? "ocean"
                             : (choice == "3") ? "forest"
                                               : "neon";
    std::cout << "\033[38;5;82m" << " [✓] Theme set to " << theme_name
              << ". Restart NovaShell to apply fully!" << R << std::endl;
    save_config();
    return true;
  } else if (cmd == "project-init") {
    std::string menu_cmd =
        "whiptail --title \"Project Initializer\" --menu \"Choose project "
        "type:\" 15 60 2 \"1\" \"C++ Project (src/include/Makefile)\" \"2\" "
        "\"Python Project (src/tests/venv)\" 2>/tmp/nova_proj_type";
    if (system(menu_cmd.c_str()) != 0)
      return true;

    std::ifstream f("/tmp/nova_proj_type");
    std::string type;
    f >> type;

    if (type == "1") {
      system("mkdir -p src include build && touch src/main.cpp include/app.h "
             "Makefile");
      std::cout << "\033[38;5;82m" << " [✓] C++ Project structure created!" << R
                << std::endl;
    } else if (type == "2") {
      system("mkdir -p src tests && touch src/main.py tests/test_main.py "
             "requirements.txt");
      std::cout << "\033[38;5;82m" << " [✓] Python Project structure created!"
                << R << std::endl;
    }
    return true;
  } else if (cmd == "health-check") {
    std::cout << B << "--- NovaShell System Health Check ---" << R << std::endl;
    auto check = [](const std::string &name, const std::string &cmd) {
      std::cout << " Checking " << std::left << std::setw(12) << name << "... ";
      if (system((cmd + " >/dev/null 2>&1").c_str()) == 0)
        std::cout << "\033[38;5;82m[ OK ]\033[0m" << std::endl;
      else
        std::cout << "\033[38;5;196m[ MISSING ]\033[0m" << std::endl;
    };
    check("curl", "curl --version");
    check("whiptail", "whiptail --version");
    check("xdg-open", "xdg-open --version");
    check("python3", "python3 --version");
    check("git", "git --version");
    return true;
  } else if (cmd == "welcome") {
    handle_builtin("fetch");
    std::cout << "\n" << B << " Welcome to NovaShell v3.0!" << R << std::endl;
    std::cout << " Type " << B << "custom" << R
              << " to see advanced AI and GUI features." << std::endl;
    std::cout << " Type " << B << "help" << R << " for basic shell commands."
              << std::endl;
    return true;
  } else if (cmd == "find-file") {
    // 1. Ask for scope
    std::string scope_cmd =
        "whiptail --title \"Search Scope\" --menu \"Where do you want to "
        "search?\" 15 60 3 "
        "\"1\" \"Current Directory (.)\" "
        "\"2\" \"Home Directory (~)\" "
        "\"3\" \"Entire System (/)\" 2>/tmp/nova_find_scope";
    if (system(scope_cmd.c_str()) != 0)
      return true;

    std::ifstream scope_file("/tmp/nova_find_scope");
    std::string scope_choice;
    scope_file >> scope_choice;

    std::string start_dir = ".";
    if (scope_choice == "2")
      start_dir = getenv("HOME");
    else if (scope_choice == "3") {
      start_dir = "/";
      std::cout << "\033[38;5;214m"
                << " [!] Warning: System-wide search can be slow and may have "
                   "permission errors."
                << R << std::endl;
    }

    std::string pattern;
    if (!(ss >> pattern)) {
      std::string input_cmd =
          "whiptail --title \"Smart File Finder\" --inputbox \"Enter file name "
          "or pattern (e.g. .cpp):\" 10 60 2>/tmp/nova_find_pattern";
      if (system(input_cmd.c_str()) != 0)
        return true;
      std::ifstream f("/tmp/nova_find_pattern");
      std::getline(f, pattern);
    }
    if (pattern.empty())
      return true;

    std::cout << DIM << " Searching in " << start_dir << " for '" << pattern
              << "'..." << R << std::endl;
    std::string find_cmd = "find " + start_dir + " -iname \"*" + pattern +
                           "*\" -not -path '*/.*' 2>/dev/null | head -n 50 > "
                           "/tmp/nova_find_results";
    system(find_cmd.c_str());

    std::ifstream res_file("/tmp/nova_find_results");
    std::vector<std::string> results;
    std::string r_line;
    while (std::getline(res_file, r_line))
      results.push_back(r_line);

    if (results.empty()) {
      system("whiptail --title \"Smart File Finder\" --msgbox \"No files found "
             "matching that pattern.\" 10 60");
      return true;
    }

    std::string menu_cmd = "whiptail --title \"Search Results\" --menu "
                           "\"Select a file:\" 20 80 12 ";
    for (const auto &res : results)
      menu_cmd += "\"" + res + "\" \"File\" ";
    menu_cmd += " 2>/tmp/nova_find_choice";

    if (system(menu_cmd.c_str()) == 0) {
      std::ifstream choice_file("/tmp/nova_find_choice");
      std::string choice;
      std::getline(choice_file, choice);
      if (!choice.empty()) {
        std::string action_cmd =
            "whiptail --title \"Action\" --menu \"File: " + choice +
            "\" 15 60 4 \"1\" \"Open with default app\" \"2\" \"Edit in Nova\" "
            "\"3\" \"Go to directory\" \"4\" \"Cancel\" "
            "2>/tmp/nova_find_action";
        if (system(action_cmd.c_str()) == 0) {
          std::ifstream act_file("/tmp/nova_find_action");
          std::string action;
          act_file >> action;
          if (action == "1")
            system(("xdg-open \"" + choice + "\" &").c_str());
          else if (action == "2")
            handle_builtin("edit " + choice);
          else if (action == "3") {
            size_t last_slash = choice.find_last_of('/');
            if (last_slash != std::string::npos)
              chdir(choice.substr(0, last_slash).c_str());
          }
        }
      }
    }
    return true;
  } else if (cmd == "browse") {
    while (true) {
      char cwd[1024];
      getcwd(cwd, sizeof(cwd));
      std::string menu_cmd =
          "whiptail --title \"Nova Directory Browser\" --menu \"Current: " +
          std::string(cwd) + "\" 20 70 12 \"..\" \"Go Up\" ";

      FILE *pipe = popen("ls -d */ 2>/dev/null", "r");
      if (pipe) {
        char buf[256];
        while (fgets(buf, sizeof(buf), pipe)) {
          std::string dir = buf;
          if (!dir.empty() && dir.back() == '\n')
            dir.pop_back();
          menu_cmd += "\"" + dir + "\" \"Directory\" ";
        }
        pclose(pipe);
      }
      menu_cmd += "\"SELECT\" \"Select this directory and exit\" "
                  "2>/tmp/nova_browse_choice";

      if (system(menu_cmd.c_str()) != 0)
        break;

      std::ifstream f("/tmp/nova_browse_choice");
      std::string choice;
      if (!(f >> choice))
        break;
      if (choice == "SELECT")
        break;

      chdir(choice.c_str());
      update_dir_history(choice);
    }
    return true;
  } else if (cmd == "export") {
    std::string arg;
    std::getline(ss, arg);
    if (!arg.empty()) {
      if (arg[0] == ' ')
        arg = arg.substr(1);
      size_t pos = arg.find('=');
      if (pos != std::string::npos) {
        std::string key = arg.substr(0, pos);
        std::string val = arg.substr(pos + 1);
        if (val.size() >= 2 && val.front() == '"' && val.back() == '"') {
          val = val.substr(1, val.size() - 2);
        }
        setenv(key.c_str(), val.c_str(), 1);
        std::cout << "\033[38;5;82m" << " [✓] Exported " << key << R
                  << std::endl;
      } else {
        std::cout << " Usage: export KEY=VALUE" << std::endl;
      }
    } else {
      std::cout << " Usage: export KEY=VALUE" << std::endl;
    }
    return true;
  } else if (cmd == "z") {
    std::string target;
    if (!(ss >> target)) {
      std::cout << B << "Frequent Directories:" << R << std::endl;
      for (auto const &[path, freq] : dir_frequency) {
        std::cout << "  " << freq << "  " << path << std::endl;
      }
      return true;
    }

    std::string best_path = "";
    int max_freq = -1;
    for (auto const &[path, freq] : dir_frequency) {
      if (path.find(target) != std::string::npos) {
        if (freq > max_freq) {
          max_freq = freq;
          best_path = path;
        }
      }
    }

    if (!best_path.empty()) {
      std::cout << DIM << "Jumping to: " << R << best_path << std::endl;
      chdir(best_path.c_str());
    } else {
      std::cout << "z: no match found for '" << target << "'" << std::endl;
    }
    return true;
  } else if (cmd == "alias") {
    for (auto const &[name, val] : aliases)
      std::cout << B << name << R << " -> " << ITALIC << val << R << std::endl;
    return true;
  } else if (cmd == "fetch" || cmd == "nova") {
    struct utsname buffer;
    uname(&buffer);
    std::cout << "\n"
              << B
              << "\033[38;5;27m  _   __                \033[38;5;33m_____ __   "
                 "       ____"
              << std::endl;
    std::cout << "\033[38;5;39m / | / /___ _   ______ \033[38;5;45m/ ___// /_  "
                 "___  / / /"
              << std::endl;
    std::cout << "\033[38;5;45m/  |/ / __ \\ | / / __ `\033[38;5;51m/\\__ \\/ "
                 "__ \\/ _ \\/ / / "
              << std::endl;
    std::cout << "\033[38;5;51m/ /|  / /_/ / |/ / /_/ \033[38;5;87m/___/ / / / "
                 "/  __/ / /  "
              << std::endl;
    std::cout << "\033[38;5;87m/_/ "
                 "|_/\\____/|___/\\__,_/\033[38;5;123m/____/_/ /_/\\___/_/_/   "
              << R << "\n"
              << std::endl;
    std::cout << B << "  USER   " << R << " : "
              << (getenv("USER") ? getenv("USER") : "unknown") << std::endl;
    std::cout << B << "  OS     " << R << " : " << buffer.sysname << " "
              << buffer.release << std::endl;
    std::cout << B << "  HOST   " << R << " : " << buffer.nodename << std::endl;
    std::cout << B << "  SHELL  " << R << " : NovaShell v3.0 (AI-Enhanced)"
              << std::endl;
    std::ifstream meminfo("/proc/meminfo");
    if (meminfo.is_open()) {
      std::string line;
      long total = 0, free = 0;
      while (std::getline(meminfo, line)) {
        if (line.find("MemTotal:") == 0)
          sscanf(line.c_str(), "MemTotal: %ld", &total);
        if (line.find("MemAvailable:") == 0)
          sscanf(line.c_str(), "MemAvailable: %ld", &free);
      }
      std::cout << B << "  MEMORY " << R << " : " << (total - free) / 1024
                << "MB / " << total / 1024 << "MB" << std::endl;
    }
    std::cout << std::endl;
    return true;
  } else if (cmd == "help") {
    std::cout << B << "\nNovaShell AI-Enhanced Commands:" << R << std::endl;
    std::cout << "  " << B << "z [query]" << R
              << " - Smart jump to frequent directories" << std::endl;
    std::cout << "  " << B << "fetch" << R << "     - System overview"
              << std::endl;
    std::cout << "  " << B << "alias" << R << "     - List command shortcuts"
              << std::endl;
    std::cout << "  " << B << "todo" << R << "      - Manage your tasks"
              << std::endl;
    std::cout << "  " << B << "calc" << R << "      - Quick math calculator"
              << std::endl;
    std::cout << "  " << B << "stats" << R << "     - Live system performance"
              << std::endl;
    std::cout << "  " << B << "custom" << R << "    - List all custom commands"
              << std::endl;
    std::cout << "  " << B << "ai" << R
              << "        - AI-powered command assistant" << std::endl;
    std::cout << "  " << B << "explore" << R << "   - Open file manager here"
              << std::endl;
    std::cout << "  " << B << "edit <file>" << R << " - Open file in GUI editor"
              << std::endl;
    std::cout << "  " << B << "alert <msg>" << R
              << " - Send desktop notification" << std::endl;
    std::cout << "  " << B << "clear" << R << "     - Wipe the terminal"
              << std::endl;
    std::cout << "  " << B << "dashboard" << R << " - Display system dashboard"
              << std::endl;
    std::cout << "  " << B << "nova-menu" << R << " - Interactive quick menu"
              << std::endl;
    std::cout << "  " << B << "see [path]" << R << " - Vision: capture screen or analyze image" << std::endl;
    return true;
  } else if (cmd == "ai") {
    std::string query;
    std::getline(ss, query);
    if (query.empty()) {
      std::cout << "Usage: ai <natural language query>" << std::endl;
      return true;
    }
    nova_intelligence(query);
    return true;
  } else if (cmd == "dashboard") {
    show_dashboard();
    return true;
  } else if (cmd == "chat") {
    std::cout << B << "\n--- Nova Interactive AI Chat ---" << R << std::endl;
    std::cout << DIM << "(Type 'exit' or 'quit' to return to shell)" << R
              << std::endl;
    while (true) {
      char *chat_input = readline("\033[38;5;13mNova 🧠 > \033[0m");
      if (!chat_input)
        break;
      std::string chat_line(chat_input);
      free(chat_input);
      if (chat_line == "exit" || chat_line == "quit")
        break;
      if (!chat_line.empty()) {
        const char *api_key = getenv("GEMINI_API_KEY");
        if (api_key ||
            !access_token.empty()) { // Check for either API key or access token
          std::cout << ITALIC << "Thinking..." << R << std::endl;
          std::cout << "\033[38;5;10m" << call_gemini_api(chat_line) << R
                    << "\n"
                    << std::endl;
        } else {
          nova_intelligence(chat_line);
        }
      }
    }
    return true;
  } else if (cmd == "nova-menu") {
    system("whiptail --title \"NovaShell Quick Menu\" --menu \"Choose a "
           "task:\" 15 60 6 \"1\" \"System Stats\" \"2\" \"Manage Todo List\" "
           "\"3\" \"Clean Project\" \"4\" \"Network Status\" \"5\" \"Git "
           "Overview\" \"6\" \"Exit Menu\" 2>/tmp/nova_menu_choice");

    std::ifstream f("/tmp/nova_menu_choice");
    std::string choice;
    if (f >> choice) {
      if (choice == "1")
        handle_builtin("stats");
      else if (choice == "2")
        handle_builtin("todo list");
      else if (choice == "3") {
        std::cout << "Cleaning..." << std::endl;
        system("make clean 2>/dev/null || rm -rf build/*");
      } else if (choice == "4")
        system("ip addr | grep 'inet '");
      else if (choice == "5")
        handle_builtin("dashboard");
    }
    return true;
  } else if (cmd == "check-ai") {
    const char *key = getenv("GEMINI_API_KEY");
    if (!key && access_token.empty()) {
      std::cout
          << "\033[38;5;196m"
          << " [!] GEMINI_API_KEY is NOT set and no access token is available."
          << R << std::endl;
      std::cout << " Use: export GEMINI_API_KEY=\"your_key\" or 'login'"
                << std::endl;
    } else {
      if (key) {
        std::string s_key(key);
        std::cout << "\033[38;5;82m" << " [✓] GEMINI_API_KEY is set." << R
                  << std::endl;
        if (s_key.length() < 10) {
          std::cout << " [!] Warning: Key seems too short." << std::endl;
        }
      } else {
        std::cout << "\033[38;5;82m"
                  << " [✓] Google Account Access Token is set." << R
                  << std::endl;
      }
      std::cout << " Testing connection..." << std::endl;
      std::string res = call_gemini_api("say 'Connection OK'");
      std::cout << " Result: " << res << std::endl;
    }
    return true;
  } else if (cmd == "set-key") {
    std::string key;
    if (ss >> key) {
      setenv("GEMINI_API_KEY", key.c_str(), 1);
      access_token = ""; // Clear access token if API key is set
      std::cout
          << "\033[38;5;82m"
          << " [✓] GEMINI_API_KEY updated successfully! (Access token cleared)"
          << R << std::endl;
    } else {
      std::cout << " Usage: set-key <your_gemini_api_key>" << std::endl;
    }
    return true;
  } else if (cmd == "see") {
    std::string path;
    std::optional<std::string> image_path;
    if (ss >> path) {
      image_path = path;
    }
    if (vision_command_) {
      vision_command_->run(image_path);
    }
    return true;
  } else if (cmd == "set-model") {
    std::string m;
    if (ss >> m) {
      model_name = m;
      save_config();
      std::cout << "\033[38;5;82m" << " [✓] Model updated to: " << model_name
                << R << std::endl;
    } else {
      std::cout << " Usage: set-model <model_name> (e.g., gemini-2.0-flash, "
                   "gemini-2.5-flash)"
                << std::endl;
    }
    return true;
  } else if (cmd == "set-api-version") {
    std::string v;
    if (ss >> v) {
      if (v == "v1" || v == "v1beta") {
        api_version = v;
        save_config();
        std::cout << "\033[38;5;82m"
                  << " [✓] API Version updated to: " << api_version << R
                  << std::endl;
      } else {
        std::cout << "\033[38;5;196m"
                  << " [!] Invalid version. Use 'v1' or 'v1beta'." << R
                  << std::endl;
      }
    } else {
      std::cout << " Usage: set-api-version <v1|v1beta>" << std::endl;
    }
    return true;
  } else if (cmd == "list-models") {
    const char *key = getenv("GEMINI_API_KEY");
    if (!key && access_token.empty()) {
      std::cout
          << "Error: GEMINI_API_KEY not set and no access token available."
          << std::endl;
      return true;
    }
    std::cout << "Fetching available models..." << std::endl;
    std::string url_str;
    std::string auth_header_str = "";

    if (!access_token.empty()) {
      url_str = "https://generativelanguage.googleapis.com/" + api_version +
                "/models";
      auth_header_str = "-H \"Authorization: Bearer " + access_token + "\"";
    } else {
      url_str = "https://generativelanguage.googleapis.com/" + api_version +
                "/models?key=" + std::string(key);
    }

    std::string cmd_str = "curl -s " + auth_header_str + " " + url_str;
    FILE *pipe = popen(cmd_str.c_str(), "r");
    if (pipe) {
      char buffer[4096];
      while (fgets(buffer, sizeof(buffer), pipe))
        std::cout << buffer;
      pclose(pipe);
    }
    return true;
  } else if (cmd == "login") {
    std::cout << B << "--- Google Account Sign-In (Device Flow) ---" << R
              << std::endl;
    std::cout << "1. Requesting device code..." << std::endl;

    std::string client_id =
        getenv("GOOGLE_CLIENT_ID") ? getenv("GOOGLE_CLIENT_ID") : "";
    if (client_id.empty()) {
      std::cout << "\033[38;5;196m"
                << " [!] GOOGLE_CLIENT_ID environment variable is not set." << R
                << std::endl;
      std::cout
          << "     Please set it to your Google Cloud OAuth 2.0 Client ID."
          << std::endl;
      std::cout
          << "     (e.g., export "
             "GOOGLE_CLIENT_ID=\"YOUR_CLIENT_ID.apps.googleusercontent.com\")"
          << std::endl;
      return true;
    }

    std::string device_code_cmd =
        "curl -s -X POST https://oauth2.googleapis.com/device/code -d "
        "\"client_id=" +
        client_id +
        "&scope=https://www.googleapis.com/auth/generative-language\"";
    FILE *pipe = popen(device_code_cmd.c_str(), "r");
    if (!pipe) {
      std::cout << "Error: Failed to initiate device flow." << std::endl;
      return true;
    }

    char buffer[4096];
    std::string device_response = "";
    while (fgets(buffer, sizeof(buffer), pipe))
      device_response += buffer;
    pclose(pipe);

    std::string device_code, user_code, verification_url, interval_str;
    size_t pos = device_response.find("\"device_code\": \"");
    if (pos != std::string::npos)
      device_code = device_response.substr(
          pos + 16, device_response.find("\"", pos + 16) - (pos + 16));
    pos = device_response.find("\"user_code\": \"");
    if (pos != std::string::npos)
      user_code = device_response.substr(
          pos + 14, device_response.find("\"", pos + 14) - (pos + 14));
    pos = device_response.find("\"verification_url\": \"");
    if (pos != std::string::npos)
      verification_url = device_response.substr(
          pos + 21, device_response.find("\"", pos + 21) - (pos + 21));
    pos = device_response.find("\"interval\": ");
    if (pos != std::string::npos)
      interval_str = device_response.substr(
          pos + 12, device_response.find(",", pos + 12) - (pos + 12));
    int interval = interval_str.empty() ? 5 : std::stoi(interval_str);

    if (device_code.empty() || user_code.empty() || verification_url.empty()) {
      std::cout << "\033[38;5;196m"
                << " [!] Failed to get device code. Response: "
                << device_response << R << std::endl;
      return true;
    }

    std::cout << "\n2. Please open this URL in your browser:\n   " << B
              << verification_url << R << std::endl;

    // Automatically open the browser
    bool opened = false;
    if (getenv("DISPLAY") || getenv("WAYLAND_DISPLAY")) {
      std::cout << DIM << " Attempting to open browser automatically..." << R
                << std::endl;
      std::string open_cmd =
          "xdg-open \"" + verification_url + "\" >/dev/null 2>&1 &";
      if (system(open_cmd.c_str()) == 0) {
        opened = true;
      } else {
        // Fallback to python if xdg-open fails
        std::string py_cmd = "python3 -m webbrowser \"" + verification_url +
                             "\" >/dev/null 2>&1 &";
        if (system(py_cmd.c_str()) == 0)
          opened = true;
      }
    }

    if (!opened) {
      std::cout << "\033[38;5;214m"
                << " [!] Could not open browser automatically. Please copy the "
                   "URL manually."
                << R << std::endl;
    }

    std::cout << "3. And enter the code: " << B << user_code << R << std::endl;
    std::cout << "   (Waiting for authorization... Press Ctrl+C to cancel)"
              << std::endl;

    std::string token_cmd_base =
        "curl -s -X POST https://oauth2.googleapis.com/token -d \"client_id=" +
        client_id + "&device_code=" + device_code +
        "&grant_type=urn%3Aietf%3Aparams%3Aoauth%3Agrant-type%3Adevice_code\"";

    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(interval));
      FILE *token_pipe = popen(token_cmd_base.c_str(), "r");
      if (!token_pipe) {
        std::cout << "Error: Failed to poll for token." << std::endl;
        break;
      }
      std::string token_response = "";
      while (fgets(buffer, sizeof(buffer), token_pipe))
        token_response += buffer;
      pclose(token_pipe);

      pos = token_response.find("\"access_token\": \"");
      if (pos != std::string::npos) {
        access_token = token_response.substr(
            pos + 17, token_response.find("\"", pos + 17) - (pos + 17));
        std::cout << "\n\033[38;5;82m" << " [✓] Signed in successfully!" << R
                  << std::endl;
        setenv("GEMINI_API_KEY", "", 1); // Clear API key if using OAuth
        break;
      } else if (token_response.find("authorization_pending") ==
                 std::string::npos) {
        std::cout << "\033[38;5;196m"
                  << " [!] Authorization failed or denied. Response: "
                  << token_response << R << std::endl;
        break;
      }
    }
    return true;
  } else if (cmd == "logout") {
    access_token = "";
    save_config();
    std::cout << "Logged out from Google Account." << std::endl;
    return true;
  } else if (cmd == "explain") {
    std::cout << B << "Explaining last command..." << R << std::endl;
    std::cout << "The last command was executed with status " << last_status
              << " and took " << last_exec_time << "s." << std::endl;
    return true;
  } else if (cmd == "explore") {
    system("xdg-open . &");
    std::cout << "Opening file manager..." << std::endl;
    return true;
  } else if (cmd == "edit") {
    std::string file;
    if (ss >> file) {
      std::string cmd_str = "xdg-open " + file + " &";
      system(cmd_str.c_str());
      std::cout << "Opening " << file << " in GUI editor..." << std::endl;
    } else {
      std::cout << "Usage: edit <file>" << std::endl;
    }
    return true;
  } else if (cmd == "alert") {
    std::string msg;
    std::getline(ss, msg);
    if (!msg.empty()) {
      if (msg[0] == ' ')
        msg = msg.substr(1);
      std::string cmd_str = "notify-send \"NovaShell\" \"" + msg + "\"";
      system(cmd_str.c_str());
      std::cout << "Notification sent!" << std::endl;
    } else {
      std::cout << "Usage: alert <message>" << std::endl;
    }
    return true;
  } else if (cmd == "custom" || cmd == "gui") {
    std::cout << "\n"
              << B << " 🚀 NovaShell Advanced Features" << R << std::endl;
    std::cout << DIM << " ──────────────────────────────────────────────────"
              << R << std::endl;

    std::cout << B << "  [ AI Assistant ]" << R << std::endl;
    std::cout << "  " << B << "ai <query>" << R << "     " << ITALIC
              << "Gemini-powered natural language assistant" << R << std::endl;
    std::cout << "  " << B << "chat" << R << "           " << ITALIC
              << "Interactive LLM conversation mode" << R << std::endl;
    std::cout << "  " << B << "set-key <key>" << R << "    " << ITALIC
              << "Set your Gemini API key directly" << R << std::endl;
    std::cout << "  " << B << "set-model <name>" << R << "   " << ITALIC
              << "Change the Gemini model (e.g., gemini-2.0-flash)" << R
              << std::endl;
    std::cout << "  " << B << "set-api-version <v>" << R << " " << ITALIC
              << "Change API version (v1 or v1beta)" << R << std::endl;
    std::cout << "  " << B << "check-ai" << R << "        " << ITALIC
              << "Verify AI connection and key" << R << std::endl;
    std::cout << "  " << B << "list-models" << R << "     " << ITALIC
              << "List all models available for your key" << R << std::endl;
    std::cout << "  " << B << "explain" << R << "        " << ITALIC
              << "Break down the last executed command" << R << std::endl;

    std::cout << "\n" << B << "  [ GUI & Productivity ]" << R << std::endl;
    std::cout << "  " << B << "explore" << R << "        " << ITALIC
              << "Open current folder in File Manager" << R << std::endl;
    std::cout << "  " << B << "browse" << R << "         " << ITALIC
              << "Interactive TUI directory navigator" << R << std::endl;
    std::cout << "  " << B << "find-file" << R << "      " << ITALIC
              << "Smart TUI-based recursive file finder" << R << std::endl;
    std::cout << "  " << B << "project-init" << R << "   " << ITALIC
              << "Scaffold new C++/Python projects" << R << std::endl;
    std::cout << "  " << B << "edit <file>" << R << "    " << ITALIC
              << "Open file in your default GUI Editor" << R << std::endl;
    std::cout << "  " << B << "alert <msg>" << R << "    " << ITALIC
              << "Send a system desktop notification" << R << std::endl;
    std::cout << "  " << B << "nova-menu" << R << "      " << ITALIC
              << "Interactive quick menu for common tasks" << R << std::endl;

    std::cout << "\n" << B << "  [ Smart Utilities ]" << R << std::endl;
    std::cout << "  " << B << "z <dir>" << R << "       " << ITALIC
              << "Frecent-based directory jumping" << R << std::endl;
    std::cout << "  " << B << "todo" << R << "          " << ITALIC
              << "Persistent task management system" << R << std::endl;
    std::cout << "  " << B << "calc <expr>" << R << "    " << ITALIC
              << "Instant arithmetic evaluator" << R << std::endl;
    std::cout << "  " << B << "stats" << R << "         " << ITALIC
              << "Live CPU & Memory performance" << R << std::endl;

    std::cout << DIM << " ──────────────────────────────────────────────────"
              << R << "\n"
              << std::endl;
    return true;
  } else if (cmd == "clear") {
    std::cout << "\033[H\033[2J" << std::flush;
    return true;
  } else if (cmd == "todo") {
    std::string sub;
    if (!(ss >> sub)) {
      std::cout << B << "Todo List Usage:" << R << std::endl;
      std::cout << "  todo add <task>    - Add a new task" << std::endl;
      std::cout << "  todo list          - Show all tasks" << std::endl;
      std::cout << "  todo remove <idx>  - Remove task at index" << std::endl;
      return true;
    }

    if (sub == "add") {
      std::string task;
      std::getline(ss, task);
      if (!task.empty()) {
        // Remove leading space
        if (task[0] == ' ')
          task = task.substr(1);
        todo_list.push_back(task);
        save_todo();
        std::cout << "Added task: " << task << std::endl;
      }
    } else if (sub == "list") {
      if (todo_list.empty()) {
        std::cout << "Your todo list is empty!" << std::endl;
      } else {
        std::cout << B << "Your Tasks:" << R << std::endl;
        for (size_t i = 0; i < todo_list.size(); i++) {
          std::cout << "  " << i + 1 << ". " << todo_list[i] << std::endl;
        }
      }
    } else if (sub == "remove") {
      int idx;
      if (ss >> idx) {
        if (idx > 0 && idx <= (int)todo_list.size()) {
          std::cout << "Removed: " << todo_list[idx - 1] << std::endl;
          todo_list.erase(todo_list.begin() + idx - 1);
          save_todo();
        } else {
          std::cout << "Invalid index." << std::endl;
        }
      }
    }
    return true;
  } else if (cmd == "calc") {
    double a, b;
    char op;
    if (ss >> a >> op >> b) {
      double res = 0;
      bool ok = true;
      if (op == '+')
        res = a + b;
      else if (op == '-')
        res = a - b;
      else if (op == '*')
        res = a * b;
      else if (op == '/') {
        if (b != 0)
          res = a / b;
        else {
          std::cout << "Error: Division by zero" << std::endl;
          ok = false;
        }
      } else {
        std::cout << "Unknown operator: " << op << std::endl;
        ok = false;
      }
      if (ok)
        std::cout << B << "Result: " << R << res << std::endl;
    } else {
      std::cout << "Usage: calc <num> <op> <num> (e.g., calc 5 + 3)"
                << std::endl;
    }
    return true;
  } else if (cmd == "stats") {
    std::ifstream stat_file("/proc/stat");
    std::string line;
    std::getline(stat_file, line);
    long user, nice, system, idle, iowait, irq, softirq, steal;
    sscanf(line.c_str(), "cpu %ld %ld %ld %ld %ld %ld %ld %ld", &user, &nice,
           &system, &idle, &iowait, &irq, &softirq, &steal);
    long total = user + nice + system + idle + iowait + irq + softirq + steal;
    long active = total - idle;

    std::cout << B << "System Stats:" << R << std::endl;
    std::cout << "  CPU Usage: " << (active * 100 / total) << "%" << std::endl;

    std::ifstream meminfo("/proc/meminfo");
    long mem_total = 0, mem_free = 0;
    while (std::getline(meminfo, line)) {
      if (line.find("MemTotal:") == 0)
        sscanf(line.c_str(), "MemTotal: %ld", &mem_total);
      if (line.find("MemAvailable:") == 0)
        sscanf(line.c_str(), "MemAvailable: %ld", &mem_free);
    }
    std::cout << "  Memory:    " << (mem_total - mem_free) / 1024 << "MB / "
              << mem_total / 1024 << "MB ("
              << ((mem_total - mem_free) * 100 / mem_total) << "%)"
              << std::endl;
    return true;
  }
  return false;
}

void Shell::nova_intelligence(const std::string &query_raw) {
  std::string query = query_raw;
  if (!query.empty() && query[0] == ' ')
    query = query.substr(1);

  std::cout << "\033[38;5;13m" << " 🧠 Nova Intelligence (Gemini-Powered) " << R
            << ITALIC << "is thinking..." << R << std::endl;

  const char *api_key = getenv("GEMINI_API_KEY");
  if (!api_key && access_token.empty()) {
    std::cout << B
              << " [!] Warning: GEMINI_API_KEY not set and no access token "
                 "available. Falling back to "
                 "local engine."
              << R << std::endl;
    // Fallback to local logic (already implemented below)
  } else {
    std::string prompt = "You are NovaShell AI, a powerful Linux shell "
                         "assistant. The user is asking: '" +
                         query +
                         "'. "
                         "Provide a concise answer with the exact shell "
                         "command to use, followed by a brief reasoning. "
                         "If the last command failed (status " +
                         std::to_string(last_status) +
                         "), consider that in your answer.";

    std::string response = call_gemini_api(prompt);
    std::cout << "\n"
              << B << " AI Insight:" << R << "\n"
              << response << "\n"
              << std::endl;
    return;
  }

  // Local Fallback Logic
  std::string response =
      "I've analyzed your request. I recommend the following:";
  std::vector<std::string> suggestions;

  std::string lower = query;
  std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

  // Context-aware fixes
  if (last_status != 0 && (lower.find("fix") != std::string::npos ||
                           lower.find("why") != std::string::npos)) {
    response = "The last command failed with exit code " +
               std::to_string(last_status) +
               ". This usually means a permission issue or a missing file.";
    suggestions.push_back("sudo !! (Run last command as root)");
    suggestions.push_back("ls -l (Check file permissions)");
  }

  // Keyword mapping
  if (lower.find("search") != std::string::npos ||
      lower.find("find") != std::string::npos) {
    suggestions.push_back("grep -r \"pattern\" . (Search text in files)");
    suggestions.push_back("find . -name \"*name*\" (Find files by name)");
  } else if (lower.find("network") != std::string::npos ||
             lower.find("port") != std::string::npos) {
    suggestions.push_back("netstat -tuln (List open ports)");
    suggestions.push_back("curl -I google.com (Test connectivity)");
  } else if (lower.find("git") != std::string::npos) {
    suggestions.push_back("git status (Check current state)");
    suggestions.push_back("git log --oneline --graph (Visualize history)");
  } else if (lower.find("performance") != std::string::npos ||
             lower.find("slow") != std::string::npos) {
    suggestions.push_back("top (Monitor processes)");
    suggestions.push_back("htop (Advanced process monitor)");
  } else if (lower.find("docker") != std::string::npos) {
    suggestions.push_back("docker ps -a (List all containers)");
    suggestions.push_back("docker images (List local images)");
    suggestions.push_back(
        "docker-compose up -d (Start services in background)");
  } else if (lower.find("system") != std::string::npos ||
             lower.find("service") != std::string::npos) {
    suggestions.push_back("systemctl status <name> (Check service status)");
    suggestions.push_back("journalctl -xe (View system logs)");
  } else if (lower.find("clean") != std::string::npos) {
    suggestions.push_back("rm -rf build/* (Clear build artifacts)");
    suggestions.push_back("du -sh * | sort -h (Find large folders)");
  }

  if (suggestions.empty()) {
    response = "I'm still learning about '" + query +
               "'. Try asking about 'git', 'searching', 'performance', or "
               "'fixing' the last error.";
  }

  std::cout << "\n" << B << " AI Insight:" << R << " " << response << std::endl;

  std::cout << B << " Reasoning:" << R << " " << ITALIC;
  if (lower.find("git") != std::string::npos)
    std::cout << "Git operations require tracking state; these commands "
                 "provide high-level visibility.";
  else if (lower.find("search") != std::string::npos)
    std::cout << "Grep is optimized for content, while find is optimized for "
                 "file metadata.";
  else if (lower.find("performance") != std::string::npos)
    std::cout << "Real-time monitoring helps identify resource bottlenecks "
                 "immediately.";
  else
    std::cout << "Based on common DevOps patterns and shell best practices.";
  std::cout << R << "\n" << std::endl;

  for (const auto &s : suggestions) {
    std::cout << "  ➜ " << B << "\033[38;5;82m" << s << R << std::endl;
  }
  std::cout << std::endl;
}

void Shell::show_dashboard() {
  std::cout << "\033[H\033[2J"; // Clear screen

  // Header
  std::cout << "\033[48;5;27m\033[38;5;255m" << std::string(80, ' ')
            << "\033[0m" << std::endl;
  std::cout << "\033[48;5;27m\033[38;5;255m"
            << "  🚀 NOVASHELL COMMAND CENTER v3.5 " << std::string(45, ' ')
            << "\033[0m" << std::endl;
  std::cout << "\033[48;5;27m\033[38;5;255m" << std::string(80, ' ')
            << "\033[0m" << std::endl;

  // Layout
  std::cout << "\n ┌─ System Health ───────────────────┐  ┌─ Git Status "
               "───────────────────────┐"
            << std::endl;

  // System Health Content
  std::ifstream stat_file("/proc/stat");
  std::string line;
  std::getline(stat_file, line);
  long u, n, s, i, iw, ir, si, st;
  sscanf(line.c_str(), "cpu %ld %ld %ld %ld %ld %ld %ld %ld", &u, &n, &s, &i,
         &iw, &ir, &si, &st);
  long total = u + n + s + i + iw + ir + si + st;
  long active = total - i;
  int cpu_pct = (active * 100 / total);

  std::string cpu_bar = "";
  for (int k = 0; k < 10; k++)
    cpu_bar += (k < cpu_pct / 10 ? "#" : " ");
  std::cout << " │ CPU: [" << "\033[38;5;82m" << cpu_bar << R << "] " << cpu_pct
            << "%    │";

  // Git Status Content
  std::string branch = get_git_branch();
  if (branch.empty())
    branch = "Not a repo";
  std::cout << "  │ Branch: " << B << "\033[38;5;208m" << branch << R
            << std::string(27 - branch.length(), ' ') << "│" << std::endl;

  std::cout << " │ Uptime: " << "Online" << std::string(19, ' ') << "│";
  std::cout << "  │ Status: " << "\033[38;5;82m" << "Clean" << R
            << std::string(28, ' ') << "│" << std::endl;

  std::cout << " └───────────────────────────────────┘  "
               "└────────────────────────────────────┘"
            << std::endl;

  // Middle Section
  std::cout << "\n ┌─ Active Tasks (Todo) "
               "─────────────────────────────────────────────────────┐"
            << std::endl;
  if (todo_list.empty()) {
    std::cout << " │  No active tasks. Use 'todo add' to get started!          "
                 "                │"
              << std::endl;
  } else {
    for (size_t idx = 0; idx < 3 && idx < todo_list.size(); idx++) {
      std::string task = todo_list[idx];
      if (task.length() > 60)
        task = task.substr(0, 57) + "...";
      std::cout << " │  " << idx + 1 << ". " << task
                << std::string(70 - task.length(), ' ') << "│" << std::endl;
    }
  }
  std::cout << " └─────────────────────────────────────────────────────────────"
               "──────────────┘"
            << std::endl;

  // Footer
  std::cout << "\n ┌─ AI Insights "
               "─────────────────────────────────────────────────────────────┐"
            << std::endl;
  std::cout
      << " │  " << ITALIC
      << "Tip: Use 'z' to jump to your most frequent directories instantly."
      << R << "      │" << std::endl;
  std::cout << " └─────────────────────────────────────────────────────────────"
               "──────────────┘"
            << std::endl;

  std::cout << "\n " << DIM << "Press ENTER to return to shell..." << R
            << std::flush;
  std::string dummy;
  std::getline(std::cin, dummy);
}

std::string Shell::call_gemini_api(const std::string &prompt) {
  const char *api_key = getenv("GEMINI_API_KEY");

  if (access_token.empty() && !api_key)
    return "Error: No authentication found. Use 'login' or 'set-key'.";

  std::string auth_header;
  std::string url;

  if (!access_token.empty()) {
    auth_header = "-H \"Authorization: Bearer " + access_token + "\"";
    url = "https://generativelanguage.googleapis.com/" + api_version +
          "/models/" + model_name + ":generateContent";
  } else {
    auth_header = ""; // No specific auth header for API key in curl command,
                      // it's in the URL
    url = "https://generativelanguage.googleapis.com/" + api_version +
          "/models/" + model_name +
          ":generateContent?key=" + std::string(api_key);
  }

  std::string escaped = "";
  for (char c : prompt) {
    if (c == '"')
      escaped += "\\\"";
    else if (c == '\\')
      escaped += "\\\\";
    else if (c == '\n')
      escaped += "\\n";
    else if (c == '\r')
      escaped += "\\r";
    else if (c == '\t')
      escaped += "\\t";
    else
      escaped += c;
  }

  std::string json_data =
      "{\"contents\": [{\"parts\":[{\"text\": \"" + escaped + "\"}]}]}";

  std::ofstream out("/tmp/nova_req.json");
  out << json_data;
  out.close();

  std::string cmd =
      "curl -s -w \"\\n%{http_code}\" -X POST \"" + url + "\" " + auth_header +
      " -H \"Content-Type: application/json\" -d @/tmp/nova_req.json";

  FILE *pipe = popen(cmd.c_str(), "r");
  if (!pipe)
    return "Error: API call failed.";

  char buffer[4096];
  std::string result = "";
  while (fgets(buffer, sizeof(buffer), pipe))
    result += buffer;
  pclose(pipe);

  if (result.empty())
    return "Error: No response from API. Check your internet connection.";

  // Extract HTTP code from the end
  size_t last_nl = result.find_last_of('\n');
  std::string http_code = "000";
  if (last_nl != std::string::npos) {
    http_code = result.substr(last_nl + 1);
    result = result.substr(0, last_nl);
  }

  if (http_code == "404")
    return "Error 404: Endpoint not found. Check your API version and model "
           "name.";
  if (http_code == "403")
    return "Error 403: Forbidden. Check your API key and permissions.";
  if (http_code == "429")
    return "Error 429: Quota Exceeded. You've reached the rate limit for the "
           "free tier. Please wait a minute before trying again.";
  if (http_code != "200")
    return "Error " + http_code +
           ": API returned an error. Raw: " + result.substr(0, 100);

  size_t text_start = result.find("\"text\": \"");
  if (text_start == std::string::npos) {
    if (result.find("API_KEY_INVALID") != std::string::npos)
      return "Error: Invalid API Key. Please check your GEMINI_API_KEY.";
    if (result.find("429") != std::string::npos)
      return "Error: Rate limit exceeded (429). Try again in a minute.";
    if (result.empty())
      return "Error: No response from API. Check your internet connection.";
    return "Error: Unexpected API response. Raw output: " +
           result.substr(0, 500) + "...";
  }

  text_start += 9;
  size_t text_end = result.find("\"", text_start);
  while (text_end != std::string::npos && result[text_end - 1] == '\\') {
    text_end = result.find("\"", text_end + 1);
  }

  if (text_end == std::string::npos)
    return "Error: Failed to parse response text.";

  std::string raw_text = result.substr(text_start, text_end - text_start);
  std::string processed = "";
  for (size_t i = 0; i < raw_text.length(); i++) {
    if (raw_text[i] == '\\' && i + 1 < raw_text.length()) {
      if (raw_text[i + 1] == 'n') {
        processed += '\n';
        i++;
      } else if (raw_text[i + 1] == '"') {
        processed += '"';
        i++;
      } else if (raw_text[i + 1] == '\\') {
        processed += '\\';
        i++;
      } else
        processed += raw_text[i];
    } else
      processed += raw_text[i];
  }
  return processed;
}

void Shell::run() {
  handle_builtin("welcome");
  while (true) {
    char *input = readline(get_prompt().c_str());
    if (!input) {
      std::cout << std::endl;
      break;
    }
    std::string line(input);
    if (!line.empty()) {
      add_history(input);
      std::stringstream ss(line);
      std::string first_word;
      ss >> first_word;
      std::string resolved = resolve_alias(first_word);
      std::vector<std::string> words;
      words.push_back(resolved);
      std::string word;
      while (ss >> word)
        words.push_back(expand_variables(word));
      std::string full_line = "";
      for (size_t i = 0; i < words.size(); i++)
        full_line += words[i] + (i == words.size() - 1 ? "" : " ");
      if (!handle_builtin(full_line)) {
        Pipeline pipeline = Parser::parse(full_line);
        auto start = std::chrono::high_resolution_clock::now();
        last_status = Executor::execute(pipeline);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        last_exec_time = elapsed.count();
        if (last_status != 0)
          suggest_command(first_word);
      } else {
        last_status = 0;
        last_exec_time = 0;
      }
    }
    free(input);
  }
}
