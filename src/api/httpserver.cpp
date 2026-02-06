#include "yhirose/httplib.h"

#include <string>
#include <iostream>
#include <thread>

void serverHTMLSetup(std::string state){
  using namespace httplib;
  // HTTP
  Server svr;
   svr.Get("/callback", [](const Request& req, Response& res) {
        res.set_content("Approved","text/plain");
        std::string code,state;
        if(req.has_param("code")){
          code = req.get_param_value("code");
          std::cout << code;
        };
        if(req.has_param("state")){
          state = req.get_param_value("state");
        };
        if (code == ""){
          res.set_content("Something went wrong","text/plain");
        } else {

        }
    });
  std::cout << " \n [HTTPserver] Starting on http://127.0.0.1:54789 \n";
  svr.listen("127.0.0.1", 54789);
  std::cout << "\n [HTTPserver] Stopped. \n";
}

