#define CPPHTTPLIB_OPENSSL_SUPPORT
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

        

        std::mutex mut;
        std::unique_lock<std::mutex> lk(mut);
        nlohmann::json j = read_config();
        string code = j["Code"];
        while(code == std::string("None"))                                   
        {
            lk.unlock();
            j = read_config();
            code = j["Code"];
            std::cout << "Waiting for code ...\n";        
            std::this_thread::sleep_for(std::chrono::milliseconds(100));   
            lk.lock();       
        }
        
        std::thread clientThread(get_access_token,j);
        if (clientThread.joinable()){clientThread.join();};
        std::cout << "\nNew thread: Client Thread, joining it\n";
        std::cout << "End of get_auth_code()\n";
    };
};
void get_access_token(nlohmann::json j){

    std::cout << "Client Thread in action\n";
    
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

    std::string temp_response;

    std::cout <<"\nSending a post request\n";


    httplib::SSLClient cli("accounts.spotify.com", 443);
    if (auto res = cli.Post("/api/token", headers, body_s, "application/x-www-form-urlencoded")){
        
        const auto status = res->status;

        switch (status) {

            case httplib::OK_200:

                std::cout << "\nSuccess\n";

            break;
            
            case httplib::BadRequest_400:

                std::cout << "\nSomething went wrong\n";
                std::cout << res->body;
            break;
        }

    } else {
        std::cout << "opa an error";
        // Check the error type
        const auto err = res.error();

        switch (err) {
            case httplib::Error::SSLConnection:
                std::cout << "SSL connection failed, SSL error: "
                        << res.ssl_error() << std::endl;
            break;

            default:
                std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
        };
    };
    std::cout << "stopping html client\n";
    if (cli.is_socket_open()){
        std::cout << "Socket is still open before cli.stop()\n";
    };
    cli.stop();
    if (cli.is_socket_open()){
        std::cout << "Socket is still open!!!!!  might crash\n";
    };

    std::cout << "End of get_access_token" << std::endl;
};

void get_refresh_token(){
    std::cout << "get refresh token";
};

