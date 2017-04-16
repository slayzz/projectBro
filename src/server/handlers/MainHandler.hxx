#ifndef WEBSOCKET_MAINHANDLER_HXX
#define WEBSOCKET_MAINHANDLER_HXX
#include "../RequestHandler.hxx"
#include "../Request.hxx"

class MainHandler : public RequestHandler {
 public:
  MainHandler();
  ~MainHandler();

  void handle(Request) override;
  std::string getUrl() const override;
};

#endif //WEBSOCKET_MAINHANDLER_HXX
