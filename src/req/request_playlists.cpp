#include <string>
#include <map>
#include <nlohmann/json.hpp>

#include "req_api.hpp"
#include "encoder.hpp"
#include "check_config.hpp"

void Get_Current_Users_Playlists(){
    using std::string;
    int limit = 50, offset = 0;

    std::map<string,string> m =
        {{"1limit",std::to_string(limit)},
        {"2offset",std::to_string(offset)}
    };

    nlohmann::json playlists = req_api::get("api.spotify.com","/v1/me/playlists?"+encode::map_ordered(m));
    write(playlists,"playlists");


    while(!playlists["next"].is_null())                                   
    {
        offset += 50;
        nlohmann::json playlists = req_api::get("api.spotify.com","/v1/me/playlists?"+encode::map_ordered(m));    
    };
    std::cout << "Total Playlists: " << playlists.value("total", 0) << std::endl;

    int counter = 0;
    std::map<int,string> map_playlists;

    for (auto x : playlists["items"]){
        counter += 1;
        map_playlists.insert_or_assign(counter,x["name"]);
    };

    for (auto x : map_playlists){
        std::cout << x.first << ". " << x.second << '\n';
    }
}

void Get_Playlists_Items(){
    nlohmann::json playlists = read("playlists");
    int pick = 0;

    std::cout << "Pick a number between 1-" << playlists.value("total", 0) << "    : \n";
    std::cin >> pick;

    std::map<int,string> map_playlists;

    int counter = 0;
    for (auto x : playlists["items"]){
        counter += 1;
        map_playlists.insert_or_assign(counter,x["name"]);
    }


    if(pick < playlists.value("total", 0)) {
        auto pair = map_playlists.find(pick);
        // s stands for selected
        if (pair != map_playlists.end()){

            std::string s_playlist = pair->second;
            std::cout << "Selected playlist name: " << s_playlist << '\n';

            // HREF LINK IN PLAYLISTS.JSON
        }

        
    }

}
