#ifndef TASK4_PARSER_H
#define TASK4_PARSER_H

#include <string>
#include <vector>

class Cmd_Parser {
public:
    void parse_cmd(int argc, char* argv[]);
    const std::string& get_option();
    const std::vector<std::string>& get_parameters();
private:
    std::string option;
    std::vector<std::string> parameters;
};

class Configs_Parser {
public:
    void parse_configs(const std::string& file_path);
    const std::vector<std::string>& get_commands();
    const std::vector<std::vector<std::string>>& get_parameters();
private:
    std::vector<std::string> commands;
    std::vector<std::vector<std::string>> parameters;
};

#endif
