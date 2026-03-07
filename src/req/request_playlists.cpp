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

    // we need playlist ID in order to get the items
    nlohmann::json playlists = read("playlists");

    std::string playlist_url = "None";
    int pick = 0;

    std::cout << "Pick a number between 1-" << playlists.value("total", 0) << " : ";
    std::cin >> pick;

    if (pick == 0){
        std::cout << "Cant pick " << pick << ", first playlist starts with 1 \n";
    } 

    else if(pick <= playlists.value("total", 0)) {
        pick -= 1;
        playlist_url = playlists["items"][pick]["items"]["href"];
        std::cout << "Playlist Name: " << playlists["items"][pick]["name"] << '\n';
        std::cout << "Total Songs in this playlist: " << playlists["items"][pick]["items"].value("total", 0) << '\n';
        std::cout << "\nFetching data ... \n\n";
    }

    else {

        std::cout << "Playlist with id " << pick << "doesnt exist \n";
    }

    // getting items starts here
    if (playlist_url != "None"){

        nlohmann::json playlist_items = req_api::get(playlist_url+"?market=PL&limit=50");
        write(playlist_items, playlists["items"][pick].value("name","Unknown")+"_items");

        while(!playlist_items["next"].is_null()){
            if (playlist_items["status"] == httplib::TooManyRequests_429){
                std::cout << "Sleeping for 15 seconds, error: TooManyRequests_429";
                std::this_thread::sleep_for(std::chrono::seconds(15));
            }
            auto new_req = req_api::get(playlist_items["next"]);

            playlist_items["next"] = new_req["next"];
            playlist_items["items"] += new_req["items"];

            write(playlist_items, playlists["items"][pick].value("name","Unknown")+"_items");
        };

        std::cout << "Updated: " << playlists["items"][pick].value("name","Unknown")+"_items.json\n";
    }

}
