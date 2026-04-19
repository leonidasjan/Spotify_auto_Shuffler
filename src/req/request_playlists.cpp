#include <string>
#include <map>
#include <charconv>
#include <nlohmann/json.hpp>

#include "req_api.hpp"
#include "encoder.hpp"
#include "check_config.hpp"

void Get_Current_Users_Playlists(){
    using std::string;
    int limit = 50, offset = 0;

    std::cout << "Pick a playlist that you want to shuffle: \n\n";

    std::map<string,string> m ={
        {"1limit",std::to_string(limit)},
        {"2offset",std::to_string(offset)}
    };

    nlohmann::json playlists = req_api::get("api.spotify.com","/v1/me/playlists?"+encode::map_ordered(m));

    // the shuffling works only when ur the owner of the playlist, so we need to exclude the followed playlists
    string owner = read("profile_data")["display_name"];

    for (size_t c = 0; c < playlists["items"].size();){

        if ( playlists["items"][c]["owner"]["display_name"] != owner ){
            playlists["items"].erase(c);
        } else { c++; } };

    write(playlists,"playlists");

    while(!playlists["next"].is_null())                                   
    {
        offset += 50;
        nlohmann::json playlists = req_api::get("api.spotify.com","/v1/me/playlists?"+encode::map_ordered(m));

        for (size_t c = 0; c < playlists["items"].size();){
            if ( playlists["items"][c]["owner"]["display_name"] != owner ){
                playlists["items"].erase(c);
            } else { c++; }
        }   
    };

    size_t counter = 0;
    for (auto x : playlists["items"]){
            counter += 1;
            std::cout << counter << ". " << x["name"] << '\n';
    };

    std::cout << std::flush << "Pick a number between 1-" << counter << " : ";
}

void Get_Playlists_Items(){
    // we need playlist ID in order to get the items
    nlohmann::json playlists = read("playlists");
    std::cin.clear();

    std::string playlist_url = "None";
    std::string s_pick = "";

    int pick;
    std::getline(std::cin, s_pick);
    auto [ptr,ec] = std::from_chars(s_pick.data(),s_pick.data() + s_pick.size(), pick);
    if (ec != std::errc{}) pick = 1;

    if (pick == 0){
        std::cout << "Cant pick " << pick << ", first playlist starts with 1 \n";
    } 

    else if(pick <= playlists.value("total", 0)) {
        pick -= 1;
        playlist_url = playlists["items"][pick]["items"]["href"];
        std::cout << "Playlist Name: " << playlists["items"][pick]["name"] << '\n';
        std::cout << "Total Songs in this playlist: " << playlists["items"][pick]["items"]["total"] << '\n';
        write("selected_playlist_id",playlists["items"][pick]["id"], "playlists");
        write("selected_playlist_snapshot_id",playlists["items"][pick]["snapshot_id"], "playlists");
        write("selected_playlist_name",playlists["items"][pick]["name"], "playlists");
        write("selected_playlist_total", playlists["items"][pick]["items"]["total"],"playlists");
    }

    else {

        std::cout << "Playlist with id " << pick << "doesnt exist \n";
    }

    // getting items starts here
    
    if(playlist_url != "None"){
        return;
    };

    nlohmann::json playlist_items;
    nlohmann::json new_req = req_api::get(playlist_url+"?market=PL&limit=50");



    playlist_items["next"] = new_req["next"];
    playlist_items["status"] = new_req["status"];

    for (auto item : new_req["items"]){
            
            nlohmann::json new_data;
            new_data["items"].push_back({
                {"img", item["item"]["album"]["images"][1]["url"]},
                {"name", item["item"]["name"]},
                {"album_url", item["item"]["album"]["images"][1]["url"]},
                {"artist_name", item["item"]["artists"][0]["name"]},
                {"added_at", item["added_at"]},
                {"uri", item["item"]["album"]["uri"]}
            });

            playlist_items["items"] += new_data["items"].back();
    };
    
    write(playlist_items, "playlists//"+playlists["items"][pick].value("name","Unknown")+"_items");

    while(!playlist_items["next"].is_null()){
        
        std::cout << "\rFetching data ... " << playlist_items["items"].size() <<'/'<< playlists["items"][pick]["items"].value("total", 0);

        auto new_req = req_api::get(playlist_items["next"]);

        for (auto item : new_req["items"]){
            
            nlohmann::json new_data;
            new_data["items"].push_back({
                {"img", item["item"]["album"]["images"][1]["url"]},
                {"name", item["item"]["name"]},
                {"album_url", item["item"]["album"]["images"][1]["url"]},
                {"artist_name", item["item"]["artists"][0]["name"]},
                {"added_at", item["added_at"]}
            });

            playlist_items["items"] += new_data["items"].back();
        };

        playlist_items["next"] = new_req["next"];

    };
    
    write(playlist_items, "playlists//"+playlists["items"][pick].value("name","Unknown")+"_items");

    std::cout << "\rFetching data ... " << playlists["items"][pick]["items"].value("total", 0) <<'/'<< playlists["items"][pick]["items"].value("total", 0) << '\n';
    std::cout << "Updated: " << playlists["items"][pick].value("name","Unknown")+"_items.json\n";


};
