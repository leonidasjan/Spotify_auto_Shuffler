#include "request_playlists.hpp"
#include "check_config.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>

void autostart_settings(){

    #ifdef _WIN32
    #define CLEAR "cls"
    #else
        #define CLEAR "clear"
    #endif

    std::system(CLEAR);
    std::cout << "===========\n";
    std::cout << "Autostart settings\n";
    std::cout << "===========\n";
    std::vector<size_t> v;
    write("autostart_playlists",v,"auth");
    while (true){
        auto playlists = read("playlists");
    Get_Current_Users_Playlists();
    std::cout << std::flush << "Pick a number between 1-" << playlists["items"].size() << " to add playlist to autostart (type d to exit): ";
    std::cin.clear();

    std::string playlist_url = "None";
    std::string s_pick = "";

    size_t pick;
    std::getline(std::cin, s_pick);
    if(s_pick == "d"){
        std::system(CLEAR);
        std::cout << "OK! These playlists will shuffle on autostart: \n";
        for (auto x : v){
            std::cout << playlists["items"][x]["name"] << '\n';
        }
        write("autostart_playlists",v,"auth");
        break;
    };
    auto [ptr,ec] = std::from_chars(s_pick.data(),s_pick.data() + s_pick.size(), pick);
    if (ec != std::errc{}) break;
    if (pick <= playlists["items"].size()){
        std::system(CLEAR);
        v.push_back(pick-1);
        std::cout << "added: " << playlists["items"][pick-1]["name"] << '\n';
    }
    }
}