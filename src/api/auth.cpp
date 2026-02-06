#include <iostream>
#include <string>
#include "log_in_spotify.hpp"
#include "encoder.hpp"
#include "httpserver.hpp"
#include <windows.h>
#include <shellapi.h>
#include <format>
#include <map>
#include <thread>
#include <chrono>
using std::string;
void authenticate_spotify(string ClientID, string ClientSecret, string Redirect_URI){
    const string scope = "user-library-read user-library-modify";
    if (ClientID == "None" || ClientSecret == "None" || Redirect_URI == "")
    {
        log_in_un_authenticated();
        std::cout << "Please try again \n";
    } else {
        std::thread serverThread(serverHTMLSetup);
        if (serverThread.joinable()){
            serverThread.detach();
        };
        std::cout << "Opening up browser...\n"; 
        std::map<string,string> m =
         {{"1response_type","code"},
         {"2client_id",ClientID},
         {"3scope",scope},
         {"4redirect_uri",Redirect_URI}
        };

        string auth_link = encode_hashmap_ordered("https://accounts.spotify.com/authorize",m);
       
        // Open the browser
        ShellExecute(0, 0, auth_link.c_str(), 0, 0 , SW_SHOW );
        std::this_thread::sleep_for(std::chrono::seconds(10));
        


    }


    //  var authOptions = {
    //   url: 'https://accounts.spotify.com/api/token',
    //   form: {
    //     code: code,
    //     redirect_uri: redirect_uri,
    //     grant_type: 'authorization_code'
    //   },
}
