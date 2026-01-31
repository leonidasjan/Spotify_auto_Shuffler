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
        // std::cout << "Before Conv: "<< auth_link << '\n';
        // auto auth_link_c = auth_link.data();
        // std::cout << "After Conv: "<<auth_link_c << "\n";
        // for (int t=0; t <auth_link.size(); t++)
        // {
        //     std::cout<<auth_link_c[t];
        // };
        std::cout << "size: " << auth_link.size() << "\n";
        for (unsigned char c : auth_link){
            std::cout <<  c << (int)c << " ";
        };
        std::cout << '\n';
        // ShellExecute(0, 0, auth_link_c, 0, 0 , SW_SHOW );
        // system(auth_link_c);


        // string test = "https://accounts.spotify.com/authorize?response_type=code&client_id=71258ec77b394b6fa8fac14ebc16c7dc&scope=user-library-read%20user-library-modify&redirect_uri=http%3A%2F%2F127.0.0.1%3A54789%2Fcallback";
        // std::cout << test << '\n';
        // std::cout << test.data() << '\n';


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
