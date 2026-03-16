#include <iostream>
#include <string>
#include <map>
#include <numeric>
#include <vector>
#include "request_playlists.hpp"
#include "check_config.hpp"
#include "nlohmann/json.hpp"
#include "req_api.hpp"

int shuffle_choice(){
    auto j = read("playlists");
    std::string pick;
    int result = 0;
    std::cout << "Do you want to shuffle that playlist? Playlist: " << j["selected_playlist_name"] << " [Y/n] : ";
    std::getline(std::cin, pick);
    
    if (pick == "" || pick == "Y" || pick == "y"){
        result = 1;
    } else {
        result = 0;
    };
    return result;
}

void shuffle(){
    auto j = read("playlists");
    auto s = j["selected_playlist_total"].get<std::string>();
    std::vector<int> v(std::stoi(s));
    std::iota(v.begin(), v.end(), 0);

    for (auto x : v){
        std::cout << x;
    };

    nlohmann::json body = {
        {"range_start",5},
        {"insert_before",0},
        {"snapshot_id",j["selected_playlist_snapshot_id"]}
    };



    std::string selected_playlist_id = j["selected_playlist_id"];
    req_api::put("api.spotify.com","/v1/playlists/" + selected_playlist_id + "/items", body);
}