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

#include <windows.h>
#include <shellapi.h>
using std::string;
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
        // httplib::Client cli("https://accounts.spotify.com/api/token");
        //  cli.Post("https://accounts.spotify.com/api/token",)
        
    };
};
void get_access_token(nlohmann::json j, std::string req_code){
    std::cout << "in func\n";
    httplib::Client cli("https://accounts.spotify.com/api");
    std::string clientid = j["ClientID"];
    std::string clientsecret = j["ClientSecret"];
    std::string auth = clientid+":"+clientsecret;

    httplib::Headers headers = {
        {"Authorization", "Basic " + base64::to_base64(auth)}
    };
    std::cout << "Authorization: Basic " + base64::to_base64(auth) << '\n';
    nlohmann::json body = {
            {"grant_type","authorization_code"},
            {"code",req_code},
            {"redirect_uri","http://127.0.0.1:54789/callback"}
        };

    std::cout << body.dump();

    auto res = cli.Post("/token", headers, body.dump(),"application/x-www-form-urlencoded");
    if (res) {
        std::cout << "Success?\n";
        std::cout << res->status << "\n";
        std::cout << res->body << "\n";
    } else {
        std::cout << "Request failed\n";
    }
};

void get_refresh_token(){
    std::cout << "get refresh token";
};


    
    //  var authOptions = {
    //   url: 'https://accounts.spotify.com/api/token',
    //   form: {
    //     code: code,
    //     redirect_uri: redirect_uri,
    //     grant_type: 'authorization_code'
    //   },
