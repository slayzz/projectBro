#ifndef WEBSOCKET_REQUESTHANDLER_HXX
#define WEBSOCKET_REQUESTHANDLER_HXX
#include <string>
#include <functional>
#include "Request.hxx"

class RequestHandler {
 public:
  virtual ~RequestHandler() {};

  virtual void handle(Request) = 0;
  virtual std::string getUrl() const = 0;
};


#endif //WEBSOCKET_REQUESTHANDLER_HXX
