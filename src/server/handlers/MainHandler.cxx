#include <iostream>
#include <event2/http.h>
#include <event2/buffer.h>
#include <fstream>
#include <cstring>
#include <evhttp.h>

#include "MainHandler.hxx"

MainHandler::MainHandler(): RequestHandler() { }

MainHandler::~MainHandler() { }

void MainHandler::handle(struct evhttp_request* request) {
  auto evb = evbuffer_new();
  const size_t bufferSize = 10024;
  const char serverName[] = "Ultra v.01";
  char buffer[bufferSize];

  int position = 0;
  std::ifstream file("../web/html/index.html");
  while(!file.eof()) {
    file.get(buffer[position++]);
  }
  buffer[position-1] = '\0';

  evbuffer_add(evb, buffer, strlen(buffer));

// Set HTTP headers
  evhttp_add_header(request->output_headers, "Server", serverName);
  evhttp_add_header(request->output_headers, "Connection", "close");
  evhttp_add_header(request->output_headers, "Content-Length",
                    std::to_string(strlen(buffer)).c_str());
  evhttp_add_header(request->output_headers, "Content-Type",
                    "text/html; charset=utf-8");

// Send reply
  evhttp_send_reply(request, HTTP_OK, "OK", evb);

  file.close();
// Free memory
  evbuffer_free(evb);
}

std::string MainHandler::getUrl() const {
  return "/";
}
