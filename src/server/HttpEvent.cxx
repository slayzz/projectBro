#include <functional>
#include <iostream>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <evhttp.h>
#include "HttpEvent.hxx"

HttpEvent::HttpEvent(Types::SOCKET& socket) : EventBase() {
  httpWatcher_ = evhttp_new(getEventBase());
  evhttp_accept_socket(httpWatcher_, socket);
  installHandlers();
  evhttp_set_gencb(httpWatcher_, &HttpEvent::defaultHandler, 0);
}

void HttpEvent::run() {
  while(true) {
    event_base_dispatch(getEventBase());
  }
}

HttpEvent::~HttpEvent() {
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
    evhttp_set_cb(httpWatcher_, handler->getUrl().c_str(), &HttpEvent::handleRequest, handler);
  }
}

void HttpEvent::handleRequest(struct evhttp_request *req, void *arg) {
  RequestHandler* handler = static_cast<RequestHandler*>(arg);
  handler->handle(req);
}

void HttpEvent::defaultHandler(struct evhttp_request *req, void *arg) {
  auto evb = evbuffer_new();
  auto uri = evhttp_request_get_uri(req);
  auto host = evhttp_request_get_host(req);
  std::cout << uri << std::endl;
  std::cout << host << std::endl;
  const char serverName[] = "Pipka";
  evbuffer_add_printf(evb, "<HTML><HEAD><TITLE>%s Page</TITLE></HEAD><BODY>\n", serverName);
// Add buffer
  evbuffer_add(evb, "<div>Lol</div>", 13);
// Add formatted text
  evbuffer_add_printf(evb, "Your request is <B>%s</B> from <B>%s</B>.<BR/>Your user agent is '%s'\n",
                      req->uri, req->remote_host, evhttp_find_header(req->input_headers, "User-Agent"));
// Add footer
  evbuffer_add_printf(evb, "</BODY></HTML>");

// Set HTTP headers
  evhttp_add_header(req->output_headers, "Server", serverName);
  evhttp_add_header(req->output_headers, "Connection", "close");

// Send reply
  evhttp_send_reply(req, HTTP_OK, "OK", evb);

// Free memory
  evbuffer_free(evb);
}
