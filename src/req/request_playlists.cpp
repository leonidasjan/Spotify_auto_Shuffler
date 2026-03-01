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
}