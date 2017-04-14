#ifndef WEBSOCKET_REQUESTHANDLER_HXX
#define WEBSOCKET_REQUESTHANDLER_HXX
#include <string>
#include <functional>

class RequestHandler {
 public:
  virtual ~RequestHandler() {};

  virtual void handle(struct evhttp_request*) = 0;
  virtual std::string getUrl() const = 0;
};

class SimpleHandler : public RequestHandler {
 public:
  SimpleHandler();
  ~SimpleHandler();

  virtual void handle(struct evhttp_request*) override;
  virtual std::string getUrl() const override;
};

#endif //WEBSOCKET_REQUESTHANDLER_HXX