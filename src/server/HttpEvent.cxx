#include <iostream>
#include <fstream>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <evhttp.h>
#include <cstring>
#include <string>
#include <cstdlib>
#include "HttpEvent.hxx"

HttpEvent::HttpEvent(Types::SOCKET& socket) : EventBase() {
  httpWatcher_ = evhttp_new(getEventBase());
  evhttp_accept_socket(httpWatcher_, socket);
  installHandlers();
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
  evhttp_set_gencb(httpWatcher_, &HttpEvent::defaultHandler, 0);
}

void HttpEvent::handleRequest(struct evhttp_request *req, void *arg) {
  RequestHandler* handler = static_cast<RequestHandler*>(arg);
  handler->handle(req);
}

void HttpEvent::defaultHandler(struct evhttp_request *req, void *arg) {
  auto evb = evbuffer_new();
  const size_t bufferSize = 10024;
  const char serverName[] = "Ultra Server";
  char buffer[bufferSize];

  int position = 0;
  std::ifstream file("../web/html/index.html");
  while(!file.eof()) {
    file.get(buffer[position++]);
  }
  buffer[position-1] = '\0';

  evbuffer_add(evb, buffer, strlen(buffer));

//  evbuffer_add_printf(evb, "<HTML><HEAD><TITLE>%s Page</TITLE></HEAD><BODY>\n", serverName);
// Add buffer
//  evbuffer_add(evb, "<div>Lol</div>", 13);
// Add formatted text
//  evbuffer_add_printf(evb, "Your request is <B>%s</B> from <B>%s</B>.<BR/>Your user agent is '%s'\n",
//                      req->uri, req->remote_host, evhttp_find_header(req->input_headers, "User-Agent"));
// Add footer
//  evbuffer_add_printf(evb, "</BODY></HTML>");

// Set HTTP headers
  evhttp_add_header(req->output_headers, "Server", serverName);
  evhttp_add_header(req->output_headers, "Connection", "close");
//  evhttp_add_header(req->output_headers, "Referrer-Policy", "no-referer");
//  evhttp_remove_header(req->output_headers, "Referer");

// Send reply
  evhttp_send_reply(req, HTTP_OK, "OK", evb);

  file.close();
// Free memory
  evbuffer_free(evb);
}
