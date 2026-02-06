#include "yhirose/httplib.h"
#include "encoder.hpp"

#include <string>
#include <iostream>
#include <thread>
#include <map>

void serverHTMLSetup(std::string state){
  using namespace httplib;
  // HTTP
  Server svr;
   svr.Get("/callback", [](const Request& req, Response& res) {
        res.set_content("Approved","text/plain");
        std::string req_code, req_state;
        if(req.has_param("code")){
          req_code = req.get_param_value("code");
          std::cout << code;
        };
        if(req.has_param("state")){
          req_state = req.get_param_value("state");
        };
        if(req.has_param("error")){
          auto req_error = req.get_param_value("error");
          res.set_content(req_error,"text/plain");
          break;
        }

        if      (req_code == "")     { res.set_content("Something went wrong","text/plain");}
        else if (req_state != state) { res.set_content("State mismatch","text/plain");}

        else {
          // TODO: request access token and figure out how this server can run in the background independenly
          // Client cli;

          // encode_hashmap_ordered("");
          // cli.send()

        };


    });
  std::cout << " \n [HTTPserver] Starting on http://127.0.0.1:54789 \n";
  svr.listen("127.0.0.1", 54789);
  std::cout << "\n [HTTPserver] Stopped. \n";
}

