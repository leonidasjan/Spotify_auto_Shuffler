#include <iostream>
#include <string>
#include "request_playlists.hpp"
#include "check_config.hpp"
#include "nlohmann/json.hpp"

int shuffle_choice(){
    auto j = read("playlists");
    std::string pick;
    int result = 0;
    std::cout << "Do you want to shuffle that playlist? [Y/n] : ";
    std::cin >> pick;
    std::cout << '\n';
    if (pick == "Y" || pick == "y" || pick == ""){
        result = 1;

    } else if (pick == "N" || pick == "n")
    {
        result = 0;
        Get_Current_Users_Playlists();
        Get_Playlists_Items();
    } else {
        std::cout << j["selected_playlist_name"];
        std::cout << "Do you want to shuffle that playlist? Playlist: " << j["selected_playlist_name"] << " [Y/n] : ";
        std::cin >> pick;
        std::cout << '\n';
        result = 0;
    }
    return result;
}

// void shuffle(){

// }