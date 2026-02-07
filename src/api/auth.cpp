#include "log_in_un_authenticated.hpp"
#include "encoder.hpp"
#include "httpserver.hpp"
#include "rand_str.hpp"

#include <iostream>
#include <string>
#include <format>
#include <map>
#include <thread>
#include <chrono>

#include <windows.h>
#include <shellapi.h>
using std::string;
void authenticate_spotify(string ClientID, string ClientSecret, string state){
    const string scope = "user-library-read user-library-modify";
    const string Redirect_URI = "http://127.0.0.1:54789/callback";
    if (ClientID == "None" || ClientSecret == "None" || state == "")
    {
        log_in_un_authenticated(state);
        std::cout << "Please try again \n";
    } else {
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

    }


    //  var authOptions = {
    //   url: 'https://accounts.spotify.com/api/token',
    //   form: {
    //     code: code,
    //     redirect_uri: redirect_uri,
    //     grant_type: 'authorization_code'
    //   },
}
