
#include <iostream>
#include <thread>
#include <mutex>

#include <winsock2.h>
#include <windows.h>

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
#include "shuffle.hpp"


int main(){

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    using std::cout, std::string;

    check_config_folders();

    // Start HTML server with unique state for auth
    write("State",randomStrGen(16),"auth");
    auto j = read("auth");
    std::jthread serverThread(serverHTML);

    std::this_thread::sleep_for(std::chrono::milliseconds(25)); // wait for HTTP server to start

    cout << "Welcome to Spotify Auto Shuffler. \n";
    
    // need to check for access token

    if (j.is_null() || j["ClientID"].is_null() || j["AccessToken"].is_null() || j["RefreshToken"].is_null()) {
        // user needs access token to continue, start auth proccess
        log_in_un_authenticated(j["State"]); 
    } else {
        // always get refresh token before continuing with requests
        get_refresh_token();
    }

    nlohmann::json Profile_Data = req_api::get("api.spotify.com","/v1/me");

    if (Profile_Data["status"] == 401){
        get_refresh_token();
    } else if (Profile_Data["status"] == 403){
        log_in_un_authenticated(j["State"]);
    }

    std::cout << "====================================\n";
    std::cout << "\nHello " << Profile_Data.value("display_name","") << "!\n\n";
    write(Profile_Data,"profile_data");

    // Request shuffle data from server

    Get_Current_Users_Playlists();
    Get_Playlists_Items();

    // Shuffle selected playlists

    auto res = shuffle_choice();
    while (!res){
        Get_Current_Users_Playlists();
        Get_Playlists_Items();
        res = shuffle_choice();
    };
    if (res == 1){
        shuffle();
    }
    std::string pick = "Y";
    while (pick == "Y" || pick == "y" || pick == ""){
        std::cout << "Want to shuffle another playlist? [Y/n]: ";
        std::getline(std::cin, pick);
        if (pick == "Y" || pick == "y" || pick == ""){
            auto res = shuffle_choice();
            while (!res){
                Get_Current_Users_Playlists();
                Get_Playlists_Items();
                res = shuffle_choice();
            };
            if (res == 1){
                shuffle();
            }
        } else {
            break;
        }
    }
    std::cout << "[Main] Requesting: html_stop\n";
    serverThread.request_stop();
    serverHTMLstop();
    return 0;
}

