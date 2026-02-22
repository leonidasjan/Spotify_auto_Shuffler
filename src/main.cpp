
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
    using std::cout, std::string;

    cout << "Welcome to Spotify Auto Shuffler. \n";

    string state = randomStrGen(16);
    std::jthread serverThread(serverHTML,state);
    check_config_folders();
    
    std::mutex m;
    m.lock();
    nlohmann::json j = read_config();
    m.unlock();

    if (j["ClientID"] == "None" || j["ClientSecret"] == "None" || j["AccessToken"] == "None" || j.is_string()) {
        // user needs access token to continue, start auth proccess
       log_in_un_authenticated(state); 
    } else {
        // always get refresh token before continuing with requests
        get_refresh_token();
    }

    cout << "\nBack to main\n";


}
