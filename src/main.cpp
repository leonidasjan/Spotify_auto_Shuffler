#include "get_vector.hpp"
#include "shuffle_alg.hpp"
#include "check_config.hpp"
#include "log_in_spotify.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


int main(){
    using std::cout; cout << "Welcome to Spotify Auto Shuffler. \n";
    check_config_folders();
    log_in_un_authenticated();
    return 0;

}
