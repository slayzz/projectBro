#include <iostream>
#include <fstream>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <evhttp.h>
#include <cstring>

#include "HttpEvent.hxx"

HttpEvent::HttpEvent(Types::SOCKET& socket) : EventBase() {
  httpWatcher_ = evhttp_new(getEventBase());
  evhttp_accept_socket(httpWatcher_, socket);
}

void HttpEvent::run() {
  installHandlers();
  while(true) {
    event_base_dispatch(getEventBase());
  }
}

HttpEvent::~HttpEvent() {
  for (auto handler : handlers_) {
    delete handler;
  }
  evhttp_free(httpWatcher_);
}

void HttpEvent::addHandler(RequestHandler* handler) {
  handlers_.push_back(handler);
}

evhttp *HttpEvent::getHttpEvent() const {
  return httpWatcher_;
}

void HttpEvent::installHandlers() {
  for(auto handler : handlers_) {
    evhttp_set_cb(httpWatcher_, handler->getUrl().c_str(),
                  &HttpEvent::handleRequest, handler);
  }
  evhttp_set_gencb(httpWatcher_, &HttpEvent::defaultHandler,
                   new StaticHandler("web"));
}

void HttpEvent::handleRequest(struct evhttp_request *req, void *arg) {
  RequestHandler* handler = static_cast<RequestHandler*>(arg);
  handler->handle(req);
}

void HttpEvent::defaultHandler(struct evhttp_request *req, void *arg) {
  StaticHandler* staticHandler = static_cast<StaticHandler*>(arg);
  try {
    staticHandler->handle(req);
  } catch(std::exception e) {
    std::cout << e.what() << std::endl;
    auto evb = evbuffer_new();
    const size_t bufferSize = 1024;
    const char serverName[] = "Ultra Server";
    char buffer[bufferSize] = "<div>Cant Find This Path</div>";


    evbuffer_add(evb, buffer, strlen(buffer));

    evhttp_add_header(req->output_headers, "Server", serverName);
    evhttp_add_header(req->output_headers, "Connection", "close");
    evhttp_add_header(req->output_headers, "Content-Length",
                      std::to_string(strlen(buffer)).c_str());

    evhttp_send_reply(req, HTTP_NOTFOUND, "Not Found", evb);
    evbuffer_free(evb);
  }
}
