#include <iostream>
#include <string>
#include "log_in_spotify.hpp"
#include "encoder.hpp"
#include <windows.h>
#include <shellapi.h>
#include <format>
#include <map>
using std::string;
void authenticate_spotify(string ClientID, string ClientSecret, string Redirect_URI){
    const string scope = "user-library-read user-library-modify";
    if (ClientID == "None" || ClientSecret == "None" || Redirect_URI == "")
    {
        log_in_un_authenticated();
        std::cout << "Please try again \n";
    } else {
        std::cout << "Opening up browser...\n"; 
        std::map<string,string> m =
         {{"1response_type","code"},
         {"2client_id",ClientID},
         {"3scope",scope},
         {"4redirect_uri",Redirect_URI}
        };

        string auth_link = encode_hashmap_ordered("https://accounts.spotify.com/authorize",m);
        std::cout << "Before Conv: "<< auth_link << '\n';
        std::cout << "After Conv: "<<auth_link.data() << "\n";
        auto auth_link_c = auth_link.data();
        for (int t=0; t <auth_link.size(); t++)
        {
            std::cout<<auth_link_c[t];
        };
        // ShellExecute(0, 0, auth_link.data(), 0, 0 , SW_SHOW );
    }
    // Open the browser
    // ShellExecute(0, 0, auth_link.c_str(), 0, 0 , SW_SHOW );
    


    //  var authOptions = {
    //   url: 'https://accounts.spotify.com/api/token',
    //   form: {
    //     code: code,
    //     redirect_uri: redirect_uri,
    //     grant_type: 'authorization_code'
    //   },
}
