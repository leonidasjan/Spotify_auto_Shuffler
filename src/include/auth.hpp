#include <string>
#ifndef AUTH_HPP
#define AUTH_HPP
using std::string;
void authenticate_spotify(
     string ClientID="None",
     string ClientSecret="None",
     string state=""
    );
#endif