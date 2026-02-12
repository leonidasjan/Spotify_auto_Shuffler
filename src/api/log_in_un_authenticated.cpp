#include <iostream>
#include <string>
#include <windows.h>
#include "auth.hpp"
#include "check_config.hpp"

void log_in_un_authenticated(std::string state){
    using std::cout , std::cin, std::string;

    string ClientID = "None";
    string ClientSecret = "None";
    const string dev_uri = "https://developer.spotify.com/dashboard";


    cout << "First of all, You need to log in to Your Spotify Account and create an app\n\n";

    // ShellExecute(0, 0, dev_uri.c_str(), 0, 0 , SW_SHOW );
    
    cout << "Log in with your Spotify Account and accept the Terms of Service\n\n";

    cout << "====================================\n";

    cout << "Click Create app,\n\n";

    cout << "Fill App name and App description however You want \n\n";

    cout << "In redirect URI, copy and paste this: http://127.0.0.1:54789/callback" << "\n\n";

    cout << "Click Save\n";
    cout << "====================================\n";
    while (ClientID == "None" || ClientSecret == "None"){

        cout << "Okay, now copy the ==Client ID== and paste it in here: ";

        cin >> ClientID;

        cout << "\n\nClick 'View Secret Key' and \n";

        cout << "\n\n Copy the ==Client Secret== and paste it in here: ";

        cin >> ClientSecret;

        cout << "\n\n Done! Now Authenticate in your browser.\n\n";
        get_auth_code( ClientID, ClientSecret, state );
    }

}
