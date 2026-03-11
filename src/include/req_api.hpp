#ifndef REQ_API_HPP
#define REQ_API_HPP
#include "yhirose/httplib.h"
#include "nlohmann/json.hpp"
#include <string>
#include <map>
namespace req_api{
    ///@{
      /**
       *  @brief  Get Request.
       *  @param  url Without https:// .
       *  @param  path Full Path to the endpoiint.
       *  @return  Body of response in JSON.
       *
       */
    nlohmann::json get(std::string url, std::string path);
    ///@}
    ///@{
    /**
     *  @brief  Get Request.
     *  @param  fullpath With https:// .
     *  @return  Body of response in JSON.
     *
     */
    nlohmann::json get(std::string fullpath);
    ///@}
    ///@{
    /**
     *  @brief  Put Request.
     *  @param  url Without https:// .
     *  @param  body < std::string , std::string >
     *  @return  Body of response in JSON.
     *
     */
    nlohmann::json post(std::string url, std::string path ,std::map<std::string,std::string> body);
    ///@}
}
#endif