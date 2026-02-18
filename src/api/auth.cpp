#include "yhirose/httplib.h"
#include "log_in_un_authenticated.hpp"
#include "encoder.hpp"
#include "httpserver.hpp"
#include "rand_str.hpp"
#include "check_config.hpp"

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

void get_access_token(nlohmann::json j);

void get_auth_code(string ClientID, string ClientSecret, string state){
    string scope = "user-library-read user-library-modify";
    const string Redirect_URI = "http://127.0.0.1:54789/callback";
    if (ClientID == "" || ClientSecret == "" || state == "")
    {
        log_in_un_authenticated(state);
        std::cout << "Please try again \n";
    } else {

        write_to_config( "ClientID", ClientID );
        write_to_config( "ClientSecret", ClientSecret );
        write_to_config( "Scope", scope);

        std::cout << "Opening up browser...\n"; 
        std::map<string,string> m =
         {{"1response_type","code"},
         {"2client_id",ClientID},
         {"3scope",scope},
         {"4redirect_uri",Redirect_URI},
         {"5state",state}
        };

        string auth_link = encode_hashmap_ordered("https://accounts.spotify.com/authorize",m);
       
        // Open the browser
        ShellExecute(0, 0, auth_link.c_str(), 0, 0 , SW_SHOW );

        

        // DO THIS ONLY IF HTML SERVER GAVE CONDITION to do it.
        std::mutex mut;
        std::unique_lock<std::mutex> lk(mut);
        nlohmann::json j = read_config();
        std::string code = j["Code"];
        while(code == std::string("None"))                                   
        {
            lk.unlock();
            j = read_config();
            code = j["Code"];
            std::cout << "Waiting for code ...\n";        
            std::this_thread::sleep_for(std::chrono::milliseconds(100));   
            lk.lock();       
        }
        
        std::jthread clientThread(get_access_token,j);
        std::cout << "\nNew thread: Client Thread, joining it\n";
        std::cout << "End of get_auth_code()\n";

    };
};
void get_access_token(nlohmann::json j){
    std::mutex m;
    std::lock_guard<std::mutex> lock(m);
    std::cout << "Client Thread in action\n";

    httplib::SSLClient cli("accounts.spotify.com", 443);


    std::string clientid = j["ClientID"];
    std::string clientsecret = j["ClientSecret"];
    std::string auth = clientid+":"+clientsecret;

    httplib::Headers headers = {
        {"Authorization", "Basic " + base64::to_base64(auth)}
    };

    std::string req_code = j["Code"];

    std::map<string,string> body =
         {{"1grant_type","authorization_code"},
         {"2redirect_uri","http://127.0.0.1:54789/callback"},
         {"3code",req_code}
        };

    std::string body_s = encode_hashmap_withoutURL(body);
    std::cout <<"Sending a post request\n";
    try
    {
        auto res = cli.Post("/api/token", headers, body_s, "application/x-www-form-urlencoded");
        if (res) {
            std::cout << "Success?\n";
            std::cout << res->status << "\n";
            std::cout << res->body << "\n";
        } else {
        std::cout << "Request failed\n";
        std::cout << res.error();
        ;}
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
};

void get_refresh_token(){
    std::cout << "get refresh token";
};

