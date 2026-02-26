#include "yhirose/httplib.h"
#ifndef REQ_API_HPP
#define REQ_API_HPP
#include "nlohmann/json.hpp"
#include <string>
#include <map>
namespace req_api{
    nlohmann::json get(std::string url, std::string path);
    nlohmann::json post(std::string url, std::map<std::string,std::string> body);
}
#endif