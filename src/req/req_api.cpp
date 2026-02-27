#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "yhirose/httplib.h"
#include "nlohmann/json.hpp"
#include "tobiaslocker/base64.hpp"
#include "check_config.hpp"
#include "encoder.hpp"
#include "auth.hpp"
#include <string>


namespace req_api {
    nlohmann::json get(std::string url, std::string path){
        using std::string;

        nlohmann::json j = read("auth");
        // Client info


        std::string auth = j["AccessToken"];
        

        //Headers

        httplib::Headers headers = {
            {"Authorization", "Bearer " + auth}
        };
        
        // //Body

        // std::map<string,string> body =
        // {{"1grant_type","authorization_code"},
        // {"2redirect_uri","http://127.0.0.1:54789/callback"},
        // {"3code",req_code}};

        // std::string body_s = encode::map_ordered(body);


        // Post request

        nlohmann::json response;



        httplib::SSLClient cli( url , 443);
        if (auto html_res = cli.Get( path , headers )){
            const auto status = html_res->status;

            switch (status) {

                case httplib::OK_200:

                    response = nlohmann::json::parse(html_res->body);

                break;
                
                case httplib::BadRequest_400:

                    std::cout << "\nSomething went wrong\n";
                    std::cout << html_res->body;

                break;

                case httplib::Unauthorized_401:

                    std::cerr << "Error: Unauthorized_401, refreshing access token!" << std::endl;
                    std::cout << html_res -> body << '\n';
                    get_refresh_token();

                break;

                case httplib::TooManyRequests_429:

                    std::cerr << "Error: TooManyRequests_429, The app has exceeded its rate limits!" << std::endl;

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
        return response;
    };

    nlohmann::json post(std::string url, std::string path, std::map<std::string,std::string> body){

        nlohmann::json j = read("auth");
        // Client info

        std::string clientid = j["ClientID"];
        std::string clientsecret = j["ClientSecret"];
        std::string auth = clientid+":"+clientsecret;
        std::string req_code = j["Code"];
        

        //Headers

        httplib::Headers headers = {
            {"Authorization", "Basic " + base64::to_base64(auth)}
        };

        

        //Headers

        // httplib::Headers headers = {
        //     {"Authorization", "Basic " + base64::to_base64(auth)}
        // };
        
        //Body



        std::string body_s = encode::map_ordered(body);


        // Post request

        nlohmann::json response;



        httplib::SSLClient cli( url , 443);
        if (auto html_res = cli.Post( path , headers , body_s , "application/x-www-form-urlencoded")){
            const auto status = html_res->status;

            switch (status) {

                case httplib::OK_200:

                    response = nlohmann::json::parse(html_res->body);

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
        return response;
    };
}

