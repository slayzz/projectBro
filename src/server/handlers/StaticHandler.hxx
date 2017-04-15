#ifndef WEBSOCKET_STATICHANDLER_HXX
#define WEBSOCKET_STATICHANDLER_HXX

#include "../RequestHandler.hxx"

class StaticHandler : public RequestHandler {
 public:
  StaticHandler(std::string);
  ~StaticHandler();
  void handle(struct evhttp_request *request) override;
  std::string getUrl() const override;
 private:
  std::string staticFolderPath_;
};

#endif //WEBSOCKET_STATICHANDLER_HXX
