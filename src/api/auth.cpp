#include "log_in_spotify.hpp"
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
void authenticate_spotify(string ClientID, string ClientSecret, string Redirect_URI){
    const string scope = "user-library-read user-library-modify";
    if (ClientID == "None" || ClientSecret == "None" || Redirect_URI == "")
    {
        log_in_un_authenticated();
        std::cout << "Please try again \n";
    } else {
        string state = randomStrGen(16);
        std::cout << "\nState:" << state << "\n";
        std::thread serverThread(serverHTMLSetup,state);
        if (serverThread.joinable()){
            serverThread.detach();
        };
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
        for (int i = 0; i < 5; ++i) {
        std::cout << "\n[Main] Doing other work... " << i << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    }


    //  var authOptions = {
    //   url: 'https://accounts.spotify.com/api/token',
    //   form: {
    //     code: code,
    //     redirect_uri: redirect_uri,
    //     grant_type: 'authorization_code'
    //   },
}
