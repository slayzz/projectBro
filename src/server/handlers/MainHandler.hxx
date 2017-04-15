#ifndef WEBSOCKET_MAINHANDLER_HXX
#define WEBSOCKET_MAINHANDLER_HXX
#include "../RequestHandler.hxx"

class MainHandler : public RequestHandler {
 public:
  MainHandler();
  ~MainHandler();

  void handle(struct evhttp_request*) override;
  std::string getUrl() const override;
};

#endif //WEBSOCKET_MAINHANDLER_HXX
