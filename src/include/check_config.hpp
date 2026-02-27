#ifndef CHECK_CONFIG_HPP
#define CHECK_CONFIG_HPP  
#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>
//First Argument is a name of the folder, second optional argument is a path where the folder is located
void make_folder(std::string folder , std::filesystem::path envpath="C:\\Users\\Leon\\AppData\\Local\\Spotify Auto Shuffler");
const char *getEnvironmentVariable(const char* varname);
// Func to check all folders in appdata
void check_config_folders();
void make_json_file(std::string name="config.json");

nlohmann::json read(const std::string name);
void write(const std::string key, const std::string pair, const std::string name);

#endif