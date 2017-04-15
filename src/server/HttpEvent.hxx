#ifndef WEBSOCKET_HTTPEVENT_HXX
#define WEBSOCKET_HTTPEVENT_HXX

#include <event2/http.h>
#include <vector>

#include "RequestHandler.hxx"
#include "EventBase.hxx"
#include "../globals.hxx"
#include "handlers/StaticHandler.hxx"

class HttpEvent : public EventBase {
 public:
  HttpEvent(Types::SOCKET&);
  ~HttpEvent();

  void addHandler(RequestHandler*);
  evhttp* getHttpEvent() const;
  void run();
 private:
  evhttp* httpWatcher_;
  std::vector<RequestHandler*> handlers_;

  static void defaultHandler(struct evhttp_request *req, void *arg);
  static void handleRequest(struct evhttp_request *req, void *arg);

  void installHandlers();

};

#endif //WEBSOCKET_HTTPEVENT_HXX
