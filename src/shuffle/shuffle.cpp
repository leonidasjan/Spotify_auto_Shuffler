#include <iostream>
#include <string>
#include "request_playlists.hpp"
#include "check_config.hpp"
#include "nlohmann/json.hpp"

int shuffle_choice(){
    auto j = read("playlists");
    std::string pick;
    int result = 0;
    std::cout << "Do you want to shuffle that playlist? Playlist: " << j["selected_playlist_name"] << " [Y/n] : ";
    std::getline(std::cin, pick);
    
    if (pick == "" || pick == "Y" || pick == "y"){
        result = 1;
        std::cout << "Picked Yes\n";
    } else if (pick == "N" || pick == "n")
    {
        result = 0;
        Get_Current_Users_Playlists();
        Get_Playlists_Items();
    } else {
        result = 0;
        std::cout << "Do you want to shuffle that playlist? Playlist: " << j["selected_playlist_name"] << " [Y/n] : ";
        std::cin >> pick;
        std::cout << '\n';
    }
    return result;
}

// void shuffle(){

// }