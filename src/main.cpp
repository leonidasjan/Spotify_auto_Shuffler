
#include <iostream>
#include <thread>
#include <mutex>
#include <nlohmann/json.hpp>
#include "get_vector.hpp"
#include "shuffle_alg.hpp"
#include "check_config.hpp"
#include "log_in_un_authenticated.hpp"
#include "auth.hpp"
#include "req_api.hpp"
#include "httpserver.hpp"
#include "rand_str.hpp"
#include "request_playlists.hpp"


int main(){
    using std::cout, std::string;

    cout << "Welcome to Spotify Auto Shuffler. \n";

    // Start HTML server with unique state for auth
    string state = randomStrGen(16);
    std::jthread serverThread(serverHTML,state);

    check_config_folders();
    
    // need to check for access token
    std::mutex m;
    m.lock();
    nlohmann::json j = read("auth");
    m.unlock();

    if (j.is_null()) {
        // user needs access token to continue, start auth proccess
        log_in_un_authenticated(state); 
    } else {
        // always get refresh token before continuing with requests
        get_refresh_token();
    }

    nlohmann::json Profile_Data = req_api::get("api.spotify.com","/v1/me");

    std::cout << "====================================\n";
    std::cout << "\nHello " << Profile_Data.value("display_name","") << "!\n";
    write(Profile_Data,"profile_data");

    Get_Current_Users_Playlists();
    // Request shuffle data from server


    // Shuffle selected playlists

}
