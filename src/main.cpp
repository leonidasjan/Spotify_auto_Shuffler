#include <iostream>
#include <thread>

#include "get_vector.hpp"
#include "shuffle_alg.hpp"
#include "check_config.hpp"
#include "rand_str.hpp"
#include "log_in_un_authenticated.hpp"
#include "httpserver.hpp"



int main(){
    using std::cout, std::string, std::thread; cout << "Welcome to Spotify Auto Shuffler. \n";
    string state = randomStrGen(16);
    std::thread serverThread(serverHTMLSetup,state);
    if (serverThread.joinable()){
        serverThread.detach();
    } else {
        std::cout << "\n\n\ncannot detach\n\n\n";
    }
    check_config_folders();
    log_in_un_authenticated(state);

}
