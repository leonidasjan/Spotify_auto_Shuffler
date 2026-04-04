#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "yhirose/httplib.h"
#include "nlohmann/json.hpp"
#include "tobiaslocker/base64.hpp"
#include "log_in_un_authenticated.hpp"
#include "check_config.hpp"
#include "encoder.hpp"
#include "auth.hpp"
#include <string>


namespace req_api {
    ///@{
      /**
       *  @brief  Get Request.
       *  @param  url Without https:// .
       *  @param  path Full Path to the endpoiint.
       *  @return  Body of response in JSON.
       *
       */
    nlohmann::json get(std::string url, std::string path){
        using std::string;

        // Client info

        nlohmann::json j = read("auth");
        std::string auth = j["AccessToken"];
        
        // Headers

        httplib::Headers headers = {
            {"Authorization", "Bearer " + auth}
        };

        // Get request

        nlohmann::json response;

        httplib::SSLClient cli( url , 443);
        if (auto html_res = cli.Get( path , headers )){
            const auto status = html_res->status;

            switch (status) {

                case httplib::OK_200:

                    response = nlohmann::json::parse(html_res->body);
                    response["status"] = httplib::OK_200;
                    
                break;
                
                case httplib::BadRequest_400:

                    std::cout << "[Get] Error: BadRequest_400\n";
                    std::cout << "[Get] " << html_res->body;
                    response["status"] = httplib::BadRequest_400;
                    log_in_un_authenticated(j["State"]);
                    
                break;

                case httplib::Unauthorized_401:

                    std::cerr << "[Get] Error: Unauthorized_401, refreshing access token!" << std::endl;
                    std::cout << html_res -> body << '\n';
                    response["status"] = httplib::Unauthorized_401;
                    if(j["AccessToken"].is_null()){
                        log_in_un_authenticated(j["State"]);
                    } else {
                        get_refresh_token();
                    }

                break;

                case httplib::Forbidden_403:

                    std::cerr << "[Get] Bad OAuth request (Probably not enough permisions). Unfortunately, re-authenticating the user won't help here." << std::endl;
                    std::cout << html_res -> body << '\n';
                    response["status"] = httplib::Forbidden_403;

                break;

                case httplib::TooManyRequests_429:
                    response["status"] = httplib::TooManyRequests_429;
                    std::cerr << "Sleeping for 15 seconds, error: TooManyRequests_429";
                    std::this_thread::sleep_for(std::chrono::seconds(15));
                break;

                case httplib::NotFound_404:

                std::cerr << "[Get] Error: NotFound_404" << std::endl;
                std::cout << html_res -> body << '\n';
                std::cout << url+path << '\n';
                response["status"] = httplib::NotFound_404;

                break;
            }

        } else {
            std::cout << "\nopa an error\n";
            // Check the error type
            const auto err = html_res.error();

            switch (err) {
                case httplib::Error::SSLConnection:

                    std::cout << "[Get] SSL connection failed, SSL error: " << html_res.ssl_error() << std::endl;
                    
                break;

                default:
                    std::cout << "[Get] HTTP error: " << httplib::to_string(err) << std::endl;
            };
        };

        
        if (cli.is_socket_open()){
            std::cout << "[Get] Socket is still open before cli.stop()\n";
        };

        cli.stop();

        if (cli.is_socket_open()){
            std::cout << "[Get] Socket is still open!!!!!  might crash\n";
        };
        return response;
    };
    ///@}
    ///@{
      /**
       *  @brief  Get Request.
       *  @param  fullpath With https:// .
       *  @return  Body of response in JSON.
       *
       */
    nlohmann::json get(std::string fullpath){
        using std::string;

        // Client info

        nlohmann::json j = read("auth");
        std::string auth = j["AccessToken"];
        
        //Headers

        httplib::Headers headers = {
            {"Authorization", "Bearer " + auth}
        };
        
        // Get request

        nlohmann::json response;

        // cli obj accepts urls only without "https://", this func gets the path from full path
        // TODO: get URL from fullpath string
        auto path = fullpath.substr(fullpath.find(".com") + 4);

        httplib::SSLClient cli( "api.spotify.com" , 443);
        if (auto html_res = cli.Get( path , headers )){
            const auto status = html_res->status;

            switch (status) {

                default:
                    std::cout << "\n=== Something went wrong ===\n";
                    std::cout << "[Get] "<< html_res->body;
                    response["status"] = status;
                    std::cout << "\n============================\n";
                break;

                case httplib::OK_200:
                    response = nlohmann::json::parse(html_res->body);
                    response["status"] = httplib::OK_200;
                break;
                
                case httplib::BadRequest_400:

                    std::cout << "[Get] Error: BadRequest_400\n";
                    std::cout << "[Get] " << html_res->body;
                    response["status"] = httplib::BadRequest_400;
                    log_in_un_authenticated(j["State"]);
                          
                break;

                case httplib::Unauthorized_401:

                    std::cerr << "[Get] Error: Unauthorized_401, refreshing access token!" << std::endl;
                    std::cout << html_res -> body << '\n';
                    response["status"] = httplib::Unauthorized_401;
                    if(j["AccessToken"].is_null()){
                        log_in_un_authenticated(j["State"]);
                    } else {
                        get_refresh_token();
                    }

                break;

                case httplib::Forbidden_403:

                    std::cerr << "[Get] Bad OAuth request (Probably not enough permisions). Unfortunately, re-authenticating the user won't help here." << std::endl;
                    std::cout << html_res -> body << '\n';
                    response["status"] = httplib::Forbidden_403;

                break;

                case httplib::TooManyRequests_429:
                    response["status"] = httplib::TooManyRequests_429;
                    std::cerr << "[Get] Sleeping for 15 seconds, error: TooManyRequests_429";
                    std::this_thread::sleep_for(std::chrono::seconds(15));
                break;

                case httplib::NotFound_404:
                    std::cerr << "[Get] Error: NotFound_404" << std::endl;
                    std::cout << html_res -> body << '\n';
                    response["status"] = httplib::NotFound_404;
                break;
            }

        } else {
            std::cout << "\nopa an error\n";
            // Check the error type
            const auto err = html_res.error();

            switch (err) {
                case httplib::Error::SSLConnection:

                    std::cout << "[Get] SSL connection failed, SSL error: " << html_res.ssl_error() << std::endl;
                    
                break;

                default:
                    std::cout << "[Get] HTTP error: " << httplib::to_string(err) << std::endl;
            };
        };

        
        if (cli.is_socket_open()){
            std::cout << "[Get] Socket is still open before cli.stop()\n";
        };

        cli.stop();

        if (cli.is_socket_open()){
            std::cout << "[Get] Socket is still open!!!!!  might crash\n";
        };
        if (response.is_null()){response = "Error";};
        return response;
    };

    ///@}

    nlohmann::json post(std::string url, std::string path, std::map<std::string,std::string> body){


        // Client info
        
        nlohmann::json j = read("auth");
        std::string auth = (j.value("ClientID","None"))+":"+(j.value("ClientSecret","None"));
        
        //Headers

        httplib::Headers headers = {
            {"Authorization", "Basic " + base64::to_base64(auth)}
        };

        //Body

        // Put func only accepts body in string form, need to convert it
        std::string body_s = encode::map_ordered(body);

        // Post request

        nlohmann::json response;
        httplib::SSLClient cli( url , 443);
        if (auto html_res = cli.Post( path , headers , body_s , "application/x-www-form-urlencoded")){
            const auto status = html_res->status;

            switch (status) {

                default:
                    std::cout << "\n=== Something went wrong ===\n";
                    std::cout << "[Post] "<< html_res->body;
                    response["status"] = status;
                    std::cout << "\n============================\n";
                break;

                case httplib::OK_200:
                    response = nlohmann::json::parse(html_res->body);
                    response["status"] = httplib::OK_200;
                    if(response.contains("access_token")){

                        write("AccessToken",response["access_token"],"auth");
                        std::cout << "Refreshed: Access Token!\n" << std::endl;
                    };

                    if(response.contains("refresh_token")){

                        write("RefreshToken",response["refresh_token"],"auth");
                        std::cout << "Refreshed: Refresh Token!\n" << std::endl;
                    };
                break;
                
                case httplib::BadRequest_400:

                    std::cout << "[Post] Error: BadRequest_400\n";
                    std::cout << "[Post] " << html_res->body;
                    response["status"] = httplib::BadRequest_400;
                    log_in_un_authenticated(j["State"]);
                    
                break;

                case httplib::Unauthorized_401:

                    std::cerr << "[Post] Error: Unauthorized_401, refreshing access token!" << std::endl;
                    std::cout << html_res -> body << '\n';
                    response["status"] = httplib::Unauthorized_401;
                    if(j["AccessToken"].is_null()){
                        log_in_un_authenticated(j["State"]);
                    } else {
                        get_refresh_token();
                    }

                break;

                case httplib::Forbidden_403:

                    std::cerr << "[Post] Bad OAuth request (Probably not enough permisions). Unfortunately, re-authenticating the user won't help here." << std::endl;
                    std::cout << html_res -> body << '\n';
                    response["status"] = httplib::Forbidden_403;

                break;

                case httplib::TooManyRequests_429:
                    response["status"] = httplib::TooManyRequests_429;
                    std::cerr << "[Post] Sleeping for 15 seconds, error: TooManyRequests_429";
                    std::this_thread::sleep_for(std::chrono::seconds(15));
                break;

                case httplib::NotFound_404:
                    std::cerr << "[Post] Error: NotFound_404" << std::endl;
                    std::cout << html_res -> body << '\n';
                    response["status"] = httplib::NotFound_404;
                break;
            }

        } else {
            std::cout << "\nopa an error\n";
            // Check the error type
            const auto err = html_res.error();

            switch (err) {
                case httplib::Error::SSLConnection:
                    std::cout << "[Post] SSL connection failed, SSL error: "
                            << html_res.ssl_error() << std::endl;
                break;

                default:
                    std::cout << "[Post] HTTP error: " << httplib::to_string(err) << std::endl;
            };
        };

        
        if (cli.is_socket_open()){
            std::cout << "[Post] Socket is still open before cli.stop()\n";
        };

        cli.stop();

        if (cli.is_socket_open()){
            std::cout << "[Post] Socket is still open!!!!!  might crash\n";
        };
        return response;
    };

    ///@{
      /**
       *  @brief  Put Request.
       *  @param  url Without https:// .
       *  @param  body < std::string , std::string >
       *  @param  path Full Path to the endpoint.
       *  @return  Body of response in JSON.
       *
       */
    nlohmann::json put(std::string url, std::string path, nlohmann::json body){

        // Client info

        nlohmann::json j = read("auth");
        std::string auth = j["AccessToken"];
        
        //Headers

        httplib::Headers headers = {
            {"Authorization", "Bearer " + auth}
        };


        nlohmann::json response;
        httplib::SSLClient cli( url , 443);
        if (auto html_res = cli.Put( path , headers , body.dump() , "application/json")){
            const auto status = html_res->status;

            switch (status) {

                default:
                    std::cout << "\n=== Something went wrong ===\n";
                    std::cout << "[Put] "<< html_res->body;
                    response["status"] = status;
                    std::cout << "\n============================\n";
                break;

                case httplib::OK_200:
                    response = nlohmann::json::parse(html_res->body);
                    response["status"] = httplib::OK_200;
                break;
                
                case httplib::BadRequest_400:

                    std::cout << "[Put] Error: BadRequest_400\n";
                    std::cout << "[Put] " << html_res->body;
                    response["status"] = httplib::BadRequest_400;
                    log_in_un_authenticated(j["State"]);
                    
                break;

                case httplib::Unauthorized_401:

                    std::cerr << "[Put] Error: Unauthorized_401, refreshing access token!" << std::endl;
                    std::cout << html_res -> body << '\n';
                    response["status"] = httplib::Unauthorized_401;
                    if(j["AccessToken"].is_null()){
                        log_in_un_authenticated(j["State"]);
                    } else {
                        get_refresh_token();
                    }

                break;

                case httplib::Forbidden_403:

                    std::cerr << "[Put] Bad OAuth request (Probably not enough permisions). Unfortunately, re-authenticating the user won't help here." << std::endl;
                    std::cout << html_res -> body << '\n';
                    response["status"] = httplib::Forbidden_403;

                break;

                case httplib::TooManyRequests_429:
                    response["status"] = httplib::TooManyRequests_429;
                    std::cerr << "[Put] Sleeping for 15 seconds, error: TooManyRequests_429";
                    std::this_thread::sleep_for(std::chrono::seconds(15));
                break;

                case httplib::NotFound_404:
                    std::cerr << "[Put] Error: NotFound_404" << std::endl;
                    std::cout << html_res -> body << '\n';
                    response["status"] = httplib::NotFound_404;
                break;
            }

        } else {
            std::cout << "\nopa an error\n";
            // Check the error type
            const auto err = html_res.error();

            switch (err) {
                case httplib::Error::SSLConnection:
                    std::cout << "[Put] SSL connection failed, SSL error: "
                            << html_res.ssl_error() << std::endl;
                break;

                default:
                    std::cout << "[Put] HTTP error: " << httplib::to_string(err) << std::endl;
            };
        };

        
        if (cli.is_socket_open()){
            std::cout << "[Put] Socket is still open before cli.stop()\n";
        };

        cli.stop();

        if (cli.is_socket_open()){
            std::cout << "[Put] Socket is still open!!!!!  might crash\n";
        };
        return response;
    };
    ///@}
}


