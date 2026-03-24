#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "yhirose/httplib.h"
#include "log_in_un_authenticated.hpp"
#include "encoder.hpp"
#include "httpserver.hpp"
#include "rand_str.hpp"
#include "check_config.hpp"
#include "req_api.hpp"

#include "tobiaslocker/base64.hpp"

#include <iostream>
#include <string>
#include <format>
#include <map>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>
#include <mutex>

#include <windows.h>
#include <shellapi.h>

using std::string;

void get_access_token();

void get_auth_code(string ClientID, string ClientSecret, string state){
    string scope = "user-library-read user-library-modify playlist-read-private playlist-modify-private playlist-modify-public";
    const string Redirect_URI = "http://127.0.0.1:54789/callback";
    if (ClientID == "" || ClientSecret == "" || state == "")
    {
        log_in_un_authenticated(state);
        std::cout << "Please try again \n";
    } else {
        write( "ClientID", ClientID, "auth" );
        write( "ClientSecret", ClientSecret, "auth" );
        write( "Scope", scope, "auth" );
        write( "Code" , "None", "auth" );

        std::cout << "Opening up browser...\n"; 
        std::map<string,string> m =
         {{"1response_type","code"},
         {"2client_id",ClientID},
         {"3scope",scope},
         {"4redirect_uri",Redirect_URI},
         {"5state",state}
        };

        string auth_link = encode::map_ordered("https://accounts.spotify.com/authorize",m);
       
        // Open the browser
        ShellExecute(0, 0, auth_link.c_str(), 0, 0 , SW_SHOW );

        

        std::mutex mut;
        std::unique_lock<std::mutex> lk(mut);
        nlohmann::json j = read("auth");
        string code = j["Code"];
        std::cout << "Waiting for code ...\n";  
        while(code == std::string("None"))                                   
        {
            lk.unlock();
            j = read("auth");
            code = j["Code"];      
            std::this_thread::sleep_for(std::chrono::milliseconds(100));   
            lk.lock();       
        }
        lk.unlock();
        // std::thread clientThread(get_access_token);
        // if (clientThread.joinable()){
        //     clientThread.join();
        // } else {std::cout << "Cant join client thread\n";};
        get_access_token();
    };
};
void get_access_token(){

    SSL_library_init(); // dont touch that
    auto j = read("auth");
    std::string Code = j["Code"];
    
    std::map<string,string> body =
    {{"1grant_type","authorization_code"},
    {"2redirect_uri","http://127.0.0.1:54789/callback"},
    {"3code",Code}};

    req_api::post("accounts.spotify.com","/api/token",body);


};

void get_refresh_token(){

    auto j = read("auth");

    std::string client = j["ClientID"];

    std::map<string,string> body =
    {{"1grant_type","refresh_token"},
    {"2refresh_token",j["RefreshToken"]},
    {"3client_id",client}};

    req_api::post("accounts.spotify.com","/api/token",body);

};

