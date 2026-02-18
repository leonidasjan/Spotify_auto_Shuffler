#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>


//For example "LOCALAPPDATA"
const char *getEnvironmentVariable(const char* varname);

//First Argument is a name of the folder, second optional argument is a path where the folder is located
void make_folder(std::string folder="New Folder", std::filesystem::path envpath=getEnvironmentVariable("LOCALAPPDATA"));

void make_json_file(std::string name="config.json", std::filesystem::path envpath=getEnvironmentVariable("LOCALAPPDATA"));

void write_to_config();

nlohmann::json read_config();

std::filesystem::path get_main_path();

void check_config_folders()
{   
    using std::filesystem::path;

    auto mainpath = get_main_path();


    if ( !std::filesystem::exists ( mainpath )) {
        make_folder("Spotify Auto Shuffler",getEnvironmentVariable("LOCALAPPDATA"));
    }

    if ( !std::filesystem::exists( mainpath / "config" )) {
        make_folder("config",mainpath);
    }

    // if ( !std::filesystem::exists( mainpath / "config" / "auth.json" )) {
    //     make_json_file("auth",mainpath / "config");
    // }

    if ( !std::filesystem::exists( mainpath / "config" / "config.json" )) {
        make_json_file("config",mainpath / "config");
    }
}

const char *getEnvironmentVariable(const char* varname) {
    const char* env_var_value = getenv(varname);

    if (env_var_value == nullptr) {
       std::cout << "Environment variable " << varname << " not found." << '\n';
    }

    return env_var_value;
}

std::filesystem::path get_main_path(){
    using std::filesystem::path;

    path envpath = getEnvironmentVariable("LOCALAPPDATA");
    path mainpath = envpath / "Spotify Auto Shuffler";
    return mainpath;
};


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
    using json = nlohmann::json;
    try
    {
        path /= name+".json";
        std::ofstream file(path);
        // write default keys
        json j = {
            {"ClientID","None"},
            {"ClientSecret","None"},
            {"AccessToken","None"},
            {"RefreshToken","None"},
            {"Scope","None"},
            {"Code","None"}
        };
        file << j.dump(4);
        file.close();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    if (std::filesystem::exists(path)){
        std::cout << "File Created: " << path << '\n';
    } else {
        std::cout << "File " << path << " did not create!! Error: ";
        std::ofstream file(path);
        // write default keys
        json j = {
            {"ClientID","None"},
            {"ClientSecret","None"},
            {"AccessToken","None"},
            {"RefreshToken","None"},
            {"Scope","None"},
            {"Code","None"}
        };
        file << j.dump(4);
        file.close();
    }
}

void write_to_config(std::string key, std::string pair){
    using std::filesystem::path; using json = nlohmann::json;
    
    auto mainpath = get_main_path();

    path filePath = mainpath / "config" / "config.json";

    if ( std::filesystem::exists( filePath )) {

        std::ifstream fileI(filePath);
        json j;
        try
        {
            
            fileI >> j;
        }
        catch(json::parse_error& e)
        {
            std::cerr << '\n' << "Json error: "<< e.what() << '\n';
        }
        
        if(j.contains(key)) {
            try
            {
                std::ofstream fileO(filePath);
                j[key] = pair;
                fileO << j.dump(4);
                fileO.close();
            }
            catch(json::parse_error& e)
            {
                std::cerr << '\n' << "Json error: "<< e.what() << '\n';
            }
            
        }
    } else {std::cerr << " This path doesnt exist?? " << filePath;}
    
}

nlohmann::json read_config(){
    using std::filesystem::path; using json = nlohmann::json;
    auto mainpath = get_main_path();
    path filePath = mainpath / "config" / "config.json";
    json j;
    if ( std::filesystem::exists( filePath )) {
        std::ifstream fileI(filePath);
        try
        {
            fileI >> j;
        }
        catch(json::parse_error& e)
        {
            std::cerr << '\n' << "Json error: "<< e.what() << '\n';
        } 
    };
    return j;
};

