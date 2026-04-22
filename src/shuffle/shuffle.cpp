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
    if (j["single"].is_null()){
        return 0;
    }
    std::string pick;
    int result = 0;
    std::string name = j["single"][0]["selected_playlist_name"];
    std::cout << "Do you want to shuffle that playlist? Playlist: " << name << " [Y/n] : ";
    std::getline(std::cin, pick);
    
    if (pick == "" || pick == "Y" || pick == "y"){
        result = 1;
    } else {
        result = 0;
    };
    return result;
}

void shuffle(int id){
    auto j = read("playlists");

    size_t total = j["items"][id]["items"]["total"];
    std::vector<int> v1(total);
    std::iota(v1.begin(), v1.end(), 0);
    
    shuffler(v1);
    size_t count = 0;
    double avg = 0;
    total = v1.size();
    for (auto a : v1){
        //eta start
        auto start = std::chrono::high_resolution_clock::now();

        auto j_n = read("playlists");
        // std::string selected_playlist_id = j_n["selected_playlist_id"];
        
            nlohmann::json body = {
                {"range_start",a},
                {"insert_before",0},
                {"snapshot_id",j["items"][id]["snapshot_id"].get<std::string>()}
            };
            auto res = req_api::put("api.spotify.com","/v1/playlists/" + j["items"][id]["id"].get<std::string>() + "/items", body);
        // check if status OK
        if (res["status"] == httplib::Forbidden_403 || res["status"] == httplib::NotFound_404){
            std::cerr << "Cant access this playlist! Is it yours? \n";
            return;
        }

        // eta end
        auto end = std::chrono::high_resolution_clock::now();
        auto d = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        double duration = d.count(); ++count; avg += duration;

        std::cout << "\r\033[KShuffling ... " << count << "/" << total << " [ETA: " << ( (avg / count) * (total - count) ) / 1000 << "s ]";
    };
    std::cout << "\r\033[KShuffling ... " << total << "/" << total << " [ETA: " << "0" << "s ]" << '\n';
    if (j["items"][id]["items"]["total"] == total){
        std::cout << "Done! Check your playlist \n";
    }



}

// void shuffle(nlohmann::json playlist_id){

// }