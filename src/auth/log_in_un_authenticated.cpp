#include <iostream>
#include <string>
#include <windows.h>
#include <thread>
#include "auth.hpp"
#include "check_config.hpp"


void log_in_un_authenticated(std::string state){
    using std::cout , std::cin, std::string;

    write("State",state,"auth");

    string ClientID = "None";
    string ClientSecret = "None";
    const string dev_uri = "https://developer.spotify.com/dashboard";

    cout << '\n';
    cout << "====================================\n";
    cout << "        FIRST TIME LOGIN\n";
    cout << "====================================\n\n";

    cout << "[1] Opening Spotify Developer page...\n\n";
    ShellExecute(0, 0, dev_uri.c_str(), 0, 0 , SW_SHOW );

    cout << "[2] Log in to your Spotify account\n";
    cout << "    - Accept the Terms of Service\n\n";

    cout << "[3] Create a new application:\n";
    cout << "    - Click 'Create App'\n";
    cout << "    - Fill in App Name and Description (anything you want)\n";
    cout << "    - In Redirect URI paste:\n";
    cout << "      http://127.0.0.1:54789/callback\n";
    cout << "    - Click 'Save'\n\n";

    cout << "====================================\n";
    cout << "        ENTER YOUR CREDENTIALS\n";
    cout << "====================================\n\n";

    while (ClientID == "None" || ClientSecret == "None"){

        cout << "[4] Paste your Client ID here:\n> ";
        std::getline(std::cin, ClientID);

        cout << "\n[5] Click 'View Client Secret' on the website\n";
        cout << "    Paste your Client Secret here:\n> ";
        std::getline(std::cin, ClientSecret);

        cout << "\n[6] Authentication will now open in your browser...\n\n";

        get_auth_code(ClientID, ClientSecret, state);
    }
}
