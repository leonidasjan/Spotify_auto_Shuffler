
#include "log_in_un_authenticated.hpp"
#include "encoder.hpp"
#include "httpserver.hpp"
#include "rand_str.hpp"
#include "check_config.hpp"

#include <iostream>
#include <string>
#include <format>
#include <map>
#include <thread>
#include <chrono>

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
void get_access_token(std::string ClientID, std::string ClientSecret, std::string scope){
    std::cout << "TODO get access token";   
};


    
    //  var authOptions = {
    //   url: 'https://accounts.spotify.com/api/token',
    //   form: {
    //     code: code,
    //     redirect_uri: redirect_uri,
    //     grant_type: 'authorization_code'
    //   },
