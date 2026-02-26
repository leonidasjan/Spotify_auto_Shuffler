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

void get_access_token();

void get_auth_code(string ClientID, string ClientSecret, string state){
    string scope = "user-library-read user-library-modify playlist-read-private";
    const string Redirect_URI = "http://127.0.0.1:54789/callback";
    if (ClientID == "" || ClientSecret == "" || state == "")
    {
        log_in_un_authenticated();
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

        string auth_link = encode::map_ordered("https://accounts.spotify.com/authorize",m);
       
        // Open the browser
        ShellExecute(0, 0, auth_link.c_str(), 0, 0 , SW_SHOW );

        

        std::mutex mut;
        std::unique_lock<std::mutex> lk(mut);
        nlohmann::json j = read_config();
        string code = j["Code"];
        std::cout << "Waiting for code ...\n";  
        while(code == std::string("None"))                                   
        {
            lk.unlock();
            j = read_config();
            code = j["Code"];      
            std::this_thread::sleep_for(std::chrono::milliseconds(100));   
            lk.lock();       
        }
        
        std::thread clientThread(get_access_token);
        if (clientThread.joinable()){clientThread.join();};
    };
};
void get_access_token(){

    SSL_library_init(); // dont touch that

    nlohmann::json j = read_config();
    
    //Client info

    std::string clientid = j["ClientID"];
    std::string clientsecret = j["ClientSecret"];
    std::string auth = clientid+":"+clientsecret;
    std::string req_code = j["Code"];
    

    //Headers

    httplib::Headers headers = {
        {"Authorization", "Basic " + base64::to_base64(auth)}
    };
    
    //Body

    std::map<string,string> body =
    {{"1grant_type","authorization_code"},
    {"2redirect_uri","http://127.0.0.1:54789/callback"},
    {"3code",req_code}};

    std::string body_s = encode::map_ordered(body);


    // Post request

    nlohmann::json response;



    httplib::SSLClient cli("accounts.spotify.com", 443);
    if (auto html_res = cli.Post("/api/token", headers, body_s, "application/x-www-form-urlencoded")){
        const auto status = html_res->status;

        switch (status) {

            case httplib::OK_200:

                response = nlohmann::json::parse(html_res->body);
                if(response.contains("access_token")){

                     write_to_config("AccessToken",response["access_token"]);
                     std::cout << "Obtained: Access Token!\n";
                };

                if(response.contains("refresh_token")){

                    write_to_config("RefreshToken",response["refresh_token"]);
                    std::cout << "Obtained: Refresh Token!\n";
                };

            break;
            
            case httplib::BadRequest_400:

                std::cout << "\nSomething went wrong\n";
                std::cout << html_res->body;
            break;
        }

    } else {
        std::cout << "\nopa an error\n";
        // Check the error type
        const auto err = html_res.error();

        switch (err) {
            case httplib::Error::SSLConnection:
                std::cout << "SSL connection failed, SSL error: "
                        << html_res.ssl_error() << std::endl;
            break;

            default:
                std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
        };
    };

    if (cli.is_socket_open()){
        std::cout << "Socket is still open before cli.stop()\n";
    };

    cli.stop();

    if (cli.is_socket_open()){
        std::cout << "Socket is still open!!!!!  might crash\n";
    };

};

void get_refresh_token(){
    nlohmann::json j = read_config();

    //Client info

    std::string clientid = j["ClientID"];
    std::string clientsecret = j["ClientSecret"];
    std::string auth = clientid+":"+clientsecret;
    

    //Headers

    httplib::Headers headers = {
        {"Authorization", "Basic " + base64::to_base64(auth)}
    };


    //Body

    std::map<string,string> body =
    {{"1grant_type","refresh_token"},
    {"2refresh_token",j["RefreshToken"]},
    {"3client_id",clientid}};

    std::string body_s = encode::map_ordered(body);

    // Post Request

    nlohmann::json response;
    
    httplib::SSLClient cli("accounts.spotify.com", 443);
    if (auto html_res = cli.Post("/api/token", headers, body_s, "application/x-www-form-urlencoded")){
        const auto status = html_res->status;

        switch (status) {

            case httplib::OK_200:

                response = nlohmann::json::parse(html_res->body);

                if(response.contains("access_token")){

                     write_to_config("AccessToken",response["access_token"]);
                     std::cout << "Refreshed: Access Token!\n";
                };

                if(response.contains("refresh_token")){

                    write_to_config("RefreshToken",response["refresh_token"]);
                    std::cout << "Refreshed: Refresh Token!\n";
                };
                

            break;
            
            case httplib::BadRequest_400:

                std::cout << "\nSomething went wrong\n";
                std::cout << html_res->body;
            break;
        }

    } else {
        std::cout << "\nopa an error\n";
        // Check the error type
        const auto err = html_res.error();

        switch (err) {
            case httplib::Error::SSLConnection:
                std::cout << "SSL connection failed, SSL error: "
                        << html_res.ssl_error() << std::endl;
            break;

            default:
                std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
        };
    };

    if (cli.is_socket_open()){
        std::cout << "Socket is still open before cli.stop()\n";
    };

    cli.stop();

    if (cli.is_socket_open()){
        std::cout << "Socket is still open!!!!!  might crash\n";
    };

};

