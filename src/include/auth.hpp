#include <string>
#ifndef AUTH_HPP
#define AUTH_HPP
using std::string;
void get_auth_code(
     string ClientID="None",
     string ClientSecret="None",
     string state=""
    );
void get_access_token(std::string);
#endif