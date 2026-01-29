#include <string>
#ifndef AUTH_HPP
#define AUTH_HPP
void authenticate_spotify(
     std::string ClientID="None",
     std::string ClientSecret="None",
     const std::string Redirect_URI = "http://127.0.0.1:54789/callback"
    );
#endif