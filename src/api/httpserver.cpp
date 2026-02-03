#include "yhirose/httplib.h"
// HTTP
httplib::Server svr;

svr.Get("/callback", [](const httplib::Request &, httplib::Response &res) {
  res.set_content("Hello World!", "text/plain");
});

svr.listen("127.0.0.1", 54789);