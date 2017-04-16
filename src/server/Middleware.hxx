#ifndef WEBSOCKET_MIDDLEWARE_HXX
#define WEBSOCKET_MIDDLEWARE_HXX

class Middleware {
 public:
  Middleware() = delete;
  virtual ~Middleware() = 0;
  void handle(struct evhttp_request*);

};

#endif //WEBSOCKET_MIDDLEWARE_HXX
