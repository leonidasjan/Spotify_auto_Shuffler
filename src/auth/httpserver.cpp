#define CPPHTTPLIB_OPENSSL_SUPPORT
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
    std::cout << "Got a response from spotify server!\n";
    std::mutex m;
    std::lock_guard<std::mutex> lock(m);
    
    std::string req_code = "";
    std::string req_state = "";

    if(req.has_param("state")){
      req_state = req.get_param_value("state");
    };

    if(req.has_param("code")){
      req_code = req.get_param_value("code");
      if      (req_code == "")     { res.set_content("Something went wrong, try again","text/plain");}
      else if (req_state != state) { res.set_content("State mismatch","text/plain");}
      else { write( "Code", req_code, "auth" ); }
      res.set_content("Approved, you can close this window now!","text/plain");
    };

    if(req.has_param("error")){
      auto req_error = req.get_param_value("error");
      res.set_content("Error: You probably copied something wrong, try again","text/plain");
      res.set_content(req_error,"text/plain");

    }


  });

  std::cout << " \n[HTTPserver] Starting on http://127.0.0.1:54789 \n";
  std::cout << "\n[HTTPserver] Started\n";
  svr.listen("127.0.0.1", 54789);
  std::cout << "\n[HTTPserver] Stopped. \n";
}

