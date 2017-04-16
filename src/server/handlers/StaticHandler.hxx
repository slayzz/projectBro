#ifndef WEBSOCKET_STATICHANDLER_HXX
#define WEBSOCKET_STATICHANDLER_HXX

#include "../RequestHandler.hxx"

class StaticHandler : public RequestHandler {
 public:
  StaticHandler(std::string);
  ~StaticHandler();
  void handle(Request) override;
  std::string getUrl() const override;
 private:
  std::string staticFolderPath_;
  void readFile(std::string uri, char* buffer);
};

#endif //WEBSOCKET_STATICHANDLER_HXX
