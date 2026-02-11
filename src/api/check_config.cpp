#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

//For example "LOCALAPPDATA"
const char *getEnvironmentVariable(const char* varname);

//First Argument is a name of the folder, second optional argument is a path where the folder is located
void make_folder(std::string folder="New Folder", std::filesystem::path envpath=getEnvironmentVariable("LOCALAPPDATA"));

void make_json_file(std::string name="config.json", std::filesystem::path envpath=getEnvironmentVariable("LOCALAPPDATA"));


void check_config_folders()
{   
    using std::filesystem::path;

    path envpath = getEnvironmentVariable("LOCALAPPDATA");
    path mainpath = envpath / "Spotify Auto Shuffler";


    if ( !std::filesystem::exists ( mainpath )) {
        make_folder("Spotify Auto Shuffler",getEnvironmentVariable("LOCALAPPDATA"));
    }

    if ( !std::filesystem::exists( mainpath / "config" )) {
        make_folder("config",mainpath);
    }

    if ( !std::filesystem::exists( mainpath / "config" / "auth.json" )) {
        make_json_file("auth",mainpath / "config");
    }

    if ( !std::filesystem::exists( mainpath / "config" / "config.json" )) {
        make_json_file("config",mainpath / "config");
    }
}

const char *getEnvironmentVariable(const char* varname) {
    const char* env_var_value = getenv(varname);

    if (env_var_value == nullptr) {
       std::cout << "Environment variable " << varname<< " not found." << '\n';
    }

    return env_var_value;
}

void make_folder(std::string folder , std::filesystem::path envpath){

    using std::cout;

    std::filesystem::path fullpath = envpath / folder;
    std::error_code err;

    int check = std::filesystem::create_directories(fullpath,err);
    
    if (check){
        cout << "Directory Created: " << fullpath << '\n';
    } else if(!check) {
        cout << "Couldnt create directory: " << fullpath;
        if (std::filesystem::exists(fullpath)) { cout << "\n This directory already exists!\n"; };
    } else {
        cout << "something went wrong\n";
    }
}

void make_json_file(std::string name,std::filesystem::path path){
    path /= name+".json";
    std::ofstream ofs(path);
    ofs.close();
    if (std::filesystem::exists(path,std::string e)){
        std::cout << "File Created: " << path;
    } else {
        std::cout << "File " << path << " did not create!! Error: " << e;
    }
}

