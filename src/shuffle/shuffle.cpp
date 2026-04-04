#include <iostream>
#include <string>
#include <map>
#include <numeric>
#include <vector>
#include <ranges>
#include <chrono>

#include "request_playlists.hpp"
#include "check_config.hpp"
#include "nlohmann/json.hpp"
#include "req_api.hpp"
#include "shuffle_alg.hpp"

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

    std::vector<int> v1(std::stoi(s));
    std::iota(v1.begin(), v1.end(), 0);
    std::vector<int> v2(std::stoi(s));
    std::iota(v2.begin(), v2.end(), 0);
    
    std::string selected_playlist_id = j["selected_playlist_id"];

    shuffler(v1);
    shuffler(v2);
    size_t count = 0;
    size_t total = v1.size();
    double avg = 0;
    for (auto [a,b] : std::views::zip(v1,v2)){
        auto j_n = read("playlists");
        
        nlohmann::json body = {
            {"range_start",a},
            {"insert_before",b},
            {"snapshot_id",j_n["selected_playlist_snapshot_id"]}
        };
        auto start = std::chrono::high_resolution_clock::now();
        auto res = req_api::put("api.spotify.com","/v1/playlists/" + selected_playlist_id + "/items", body);
        if (res["status"] == httplib::OK_200){
            write("selected_playlist_snapshot_id",res["snapshot_id"],"playlists");
        } else if (res["status"] == httplib::Forbidden_403 || res["status"] == httplib::NotFound_404){
            auto end = std::chrono::high_resolution_clock::now(); // to measure eta when err
            std::cerr << "Cant access this playlist! Is it yours? \n";
            return;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto d = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        double duration = d.count();
        ++count;
        avg += duration;

        std::cout << "\r\033[KShuffling ... " << count << "/" << total << " [ETA: " << ( (avg / count) * (total - count) ) / 1000 << "s ]";
    };
    std::cout << "\r\033[KShuffling ... " << total << "/" << total << " [ETA: " << "0" << "s ]" << '\n';
    std::cout << "Done! Check your playlist \n";



}