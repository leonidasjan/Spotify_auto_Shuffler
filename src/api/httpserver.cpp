#include "yhirose/httplib.h"
#include "encoder.hpp"
#include "tobiaslocker/base64.hpp" 
#include "auth.hpp"
#include "check_config.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <map>
#include <mutex>

void serverHTML(std::string state){
  using namespace httplib;
  // HTTP
  Server svr;

  svr.Get("/callback", [state](const Request& req, Response& res) {
    
    std::string req_code = "";
    std::string req_state = "";

    

    if(req.has_param("code")){
      req_code = req.get_param_value("code");
      if      (req_code == "")     { res.set_content("Something went wrong","text/plain");}
      else if (req_state != state) { res.set_content("State mismatch","text/plain");}
    };
    if(req.has_param("state")){
      req_state = req.get_param_value("state");
    };
    if(req.has_param("error")){
      auto req_error = req.get_param_value("error");
      res.set_content(req_error,"text/plain");
    } else {
      // std::mutex m;
      // std::lock_guard<std::mutex> lock(m);
      res.set_content("Approved","text/plain");

      write_to_config("AccessToken",req_code);


      get_access_token(read_config(),req_code);
    }


  });

  std::cout << " \n [HTTPserver] Starting on http://127.0.0.1:54789 \n";
  std::cout << "\n [HTTPserver] Started\n";
  svr.listen("127.0.0.1", 54789);
  std::cout << "\n [HTTPserver] Stopped. \n";
}

