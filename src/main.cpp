
#include <iostream>
#include <thread>
#include <mutex>
#include <nlohmann/json.hpp>
#include "get_vector.hpp"
#include "shuffle_alg.hpp"
#include "check_config.hpp"
#include "rand_str.hpp"
#include "log_in_un_authenticated.hpp"
#include "httpserver.hpp"
#include "auth.hpp"


int main(){
    using std::cout, std::string, std::thread;
    cout << "Welcome to Spotify Auto Shuffler. \n";
    string state = randomStrGen(16);
    std::thread serverThread(serverHTML,state);
    check_config_folders();
    std::mutex m;
    m.lock();
    nlohmann::json j = read_config();
    std::string req_code = j["AccessToken"];
    m.unlock();
    //  || j["ClientID"] != ""  || j["RefreshToken"] != "" 
    if (j["ClientID"] == "" || j["ClientSecret"] == "null" || j.is_string()) {
       log_in_un_authenticated(state); 
    } else {
        // std::cout << "authenticated, getting access token \n";
       get_access_token(j,req_code);

    }

    
    if (serverThread.joinable()){
        serverThread.join();
    };

}
